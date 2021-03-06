/*ckwg +29
 * Copyright 2012-2013 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "statistics.h"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/variance.hpp>

/**
 * \file statistics.cxx
 *
 * \brief Implementation of a statistics class.
 */

namespace ba = boost::accumulators;

namespace sprokit
{

class statistics::priv
{
  public:
    priv();
    ~priv();

    typedef ba::accumulator_set<data_point_t, ba::stats
      < ba::tag::count
      , ba::tag::sum
      , ba::tag::min
      , ba::tag::max
      , ba::tag::mean
      , ba::tag::median(ba::with_p_square_quantile)
      , ba::tag::variance(ba::lazy)
      > > stats_t;

    stats_t acc;
    data_points_t data;
};

statistics
::statistics(data_points_t const& pts)
  : d(new priv)
{
  add_points(pts);
}

statistics
::~statistics()
{
}

void
statistics
::add_point(data_point_t pt)
{
  d->acc(pt);
  d->data.push_back(pt);
}

void
statistics
::add_points(data_points_t const& pts)
{
  d->acc = std::for_each(pts.begin(), pts.end(), d->acc);
  d->data.insert(d->data.end(), pts.begin(), pts.begin());
}

statistics::data_points_t
statistics
::data() const
{
  return d->data;
}

size_t
statistics
::count() const
{
  return ba::count(d->acc);
}

statistics::data_point_t
statistics
::sum() const
{
  return ba::sum(d->acc);
}

statistics::data_point_t
statistics
::minimum() const
{
  return ba::min(d->acc);
}

statistics::data_point_t
statistics
::maximum() const
{
  return ba::max(d->acc);
}

statistics::data_point_t
statistics
::range() const
{
  return (maximum() - minimum());
}

double
statistics
::mean() const
{
  return ba::mean(d->acc);
}

double
statistics
::median() const
{
  return ba::median(d->acc);
}

double
statistics
::variance() const
{
  return ba::variance(d->acc);
}

double
statistics
::standard_deviation() const
{
  return sqrt(variance());
}

statistics::priv
::priv()
  : acc()
  , data()
{
}

statistics::priv
::~priv()
{
}

}
