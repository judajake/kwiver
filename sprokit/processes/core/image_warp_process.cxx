/*ckwg +29
 * Copyright 2017 by Kitware, Inc.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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

#include "image_warp_process.h"

#include <vital/algo/warp_image.h>

#include <sprokit/processes/kwiver_type_traits.h>
#include <sprokit/pipeline/process_exception.h>

namespace kwiver {

create_config_trait( algo, std::string, "", "Algorithm configuration subblock" );

//----------------------------------------------------------------
// Private implementation class
class image_warp_process::priv
{
public:
  priv();
  ~priv();

   vital::algo::warp_image_sptr m_algo;

}; // end priv class


// ==================================================================
image_warp_process::
image_warp_process( kwiver::vital::config_block_sptr const& config )
  : process( config ),
    d( new image_warp_process::priv )
{
  // Attach our logger name to process logger
  attach_logger( kwiver::vital::get_logger( name() ) ); // could use a better approach

  make_ports();
  make_config();
}


image_warp_process::
~image_warp_process()
{
}


// ------------------------------------------------------------------
void
image_warp_process::
_configure()
{
  vital::config_block_sptr algo_config = get_config();

  // Check config so it will give run-time diagnostic of config problems
  if ( ! vital::algo::warp_image::check_nested_algo_configuration_using_trait( algo, algo_config ) )
  {
    throw sprokit::invalid_configuration_exception( name(), "Configuration check failed." );
  }

  vital::algo::warp_image::set_nested_algo_configuration_using_trait( algo, algo_config, d->m_algo );

  if ( ! d->m_algo )
  {
    throw sprokit::invalid_configuration_exception( name(), "Unable to create warping algorithm" );
  }

  vital::algo::warp_image::get_nested_algo_configuration_using_trait( algo, algo_config, d->m_algo );

}


// ------------------------------------------------------------------
void
image_warp_process::
_step()
{
  auto input = grab_from_port_using_trait( image );
  auto homog = grab_from_port_using_trait( homography );

  vital::image_container_sptr result;
  d->m_algo->warp( input, result, homog );

  push_to_port_using_trait( image, result );
}


// ------------------------------------------------------------------
void
image_warp_process::
make_ports()
{
  // Set up for required ports
  sprokit::process::port_flags_t required;
  sprokit::process::port_flags_t optional;

  required.insert( flag_required );

  // -- input --
  declare_input_port_using_trait( image, required );
  declare_input_port_using_trait( homography, required );

  // -- output --
  declare_output_port_using_trait( image, optional );
}


// ------------------------------------------------------------------
void
image_warp_process::
make_config()
{
  declare_config_using_trait( algo );
}


// ================================================================
image_warp_process::priv
::priv()
{
}


image_warp_process::priv
::~priv()
{
}

} // end namespace kwiver