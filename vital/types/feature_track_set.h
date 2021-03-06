/*ckwg +29
 * Copyright 2013-2017 by Kitware, Inc.
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

/**
 * \file
 * \brief Header file for \link kwiver::vital::feature_track_set feature_track_set
 *        \endlink
 */

#ifndef VITAL_FEATURE_TRACK_SET_H_
#define VITAL_FEATURE_TRACK_SET_H_

#include "descriptor_set.h"
#include "feature_set.h"
#include "track_set.h"

#include <vital/vital_export.h>
#include <vital/vital_config.h>
#include <vital/vital_types.h>

#include <vector>
#include <memory>

namespace kwiver {
namespace vital {

class feature_track_state;
typedef std::shared_ptr<feature_track_state> feature_track_state_sptr;

/// A derived track_state for feature tracks
class VITAL_EXPORT feature_track_state : public track_state
{
public:
  /// Constructor
  explicit feature_track_state( frame_id_t frame,
                                feature_sptr f = nullptr,
                                descriptor_sptr d = nullptr )
    : track_state( frame )
    , feature(f)
    , descriptor(d)
  { }

  /// Clone the track state (polymorphic copy constructor)
  virtual track_state_sptr clone() const
  {
    return std::make_shared<feature_track_state>(*this);
  }

  feature_sptr feature;
  descriptor_sptr descriptor;
};



class feature_track_set_frame_data;
typedef std::shared_ptr<feature_track_set_frame_data> feature_track_set_frame_data_sptr;

/// A derived track_state_frame_data for feature tracks
class VITAL_EXPORT feature_track_set_frame_data
 : public track_set_frame_data
{
public:
  // Dynamic copy constructor
  virtual track_set_frame_data_sptr clone() const
  {
    return std::make_shared<feature_track_set_frame_data>(*this);
  }

  bool is_keyframe;
};


class feature_info {
public:
  feature_set_sptr features;
  descriptor_set_sptr descriptors;
  std::vector<track_sptr> corresponding_tracks;
};

typedef std::shared_ptr< feature_info> feature_info_sptr;

class feature_track_set;
/// Shared pointer for feature_track_set type
typedef std::shared_ptr< feature_track_set > feature_track_set_sptr;

/// A collection of 2D feature point tracks
class VITAL_EXPORT feature_track_set : public track_set
{
public:
  /// Default Constructor
  /**
   * \note implementation defaults to simple_track_set_implementation
   */
  feature_track_set();

  /// Constructor specifying the implementation
  feature_track_set(std::unique_ptr<track_set_implementation> impl);

  /// Constructor from a vector of tracks
  /**
   * \note implementation defaults to simple_track_set_implementation
   */
  feature_track_set(std::vector< track_sptr > const& tracks);

  /// Destructor
  virtual ~feature_track_set() = default;

  /**
  * \note returns a deep copy of the feature_track_set
  */
  virtual track_set_sptr clone() const;

  /// Return the set of features in tracks on the last frame
  virtual feature_set_sptr last_frame_features() const;

  /// Return the set of descriptors in tracks on the last frame
  virtual descriptor_set_sptr last_frame_descriptors() const;

  /// Return the set of features in all tracks for the given frame.
  /**
   * \param [in] offset the frame offset for selecting the target frame.
   *                    Positive number are absolute frame numbers while
   *                    negative numbers are relative to the last frame.  For
   *                    example, offset of -1 refers to the last frame and is
   *                    the default.
   *
   * \returns a feature_set_sptr for all features on the give frame.
   */
  virtual feature_set_sptr frame_features( frame_id_t offset = -1 ) const;

  /// Return the set of descriptors in all tracks for the given frame.
  /**
   * \param [in] offset the frame offset for selecting the target frame.
   *                    Positive number are absolute frame numbers while
   *                    negative numbers are relative to the last frame.  For
   *                    example, offset of -1 refers to the last frame and is
   *                    the default.
   *
   * \returns a descriptor_set_sptr for all features on the give frame.
   */
  virtual descriptor_set_sptr frame_descriptors( frame_id_t offset = -1 ) const;

  /// Return a map of all feature_track_set_frame_data
  /**
   * This function is similar to \c all_frame_data() except that it checks
   * the type of the frame data and dynamically casts it to the specialized
   * frame data for feature_track_set.  Any frame data of a different type
   * is not included in this ouput.
   */
  virtual std::map<frame_id_t, feature_track_set_frame_data_sptr>
    all_feature_frame_data() const;

  /// Return the set of all keyframes in the track set
  /**
   * Keyframes are designated as frames which have an associated
   * feature_track_set_frame_data marked with is_keyframe == true
   */
  virtual std::set<frame_id_t> keyframes() const;

  virtual feature_info_sptr frame_feature_info(frame_id_t offset = -1,
    bool only_features_with_descriptors = true) const;

  /// Return the additional data associated with all feature tracks on the given frame
  feature_track_set_frame_data_sptr feature_frame_data(frame_id_t offset = -1) const;

};


} } // end namespace vital

#endif // VITAL_FEATURE_TRACK_SET_H_
