// ======================================================================== //
// Copyright 2009-2017 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "../../../common/sys/platform.h"
#include "../../../common/sys/ref.h"
#include "../../../common/sys/intrinsics.h"
#include "../../../common/sys/sysinfo.h"
#include "../../../common/sys/atomic.h"
#include "../../../common/sys/vector.h"
#include "../../../common/sys/string.h"

#include "../../../common/math/math.h"
#include "../../../common/math/vec2.h"
#include "../../../common/math/vec3.h"
#include "../../../common/math/vec4.h"
#include "../../../common/math/bbox.h"
#include "../../../common/math/affinespace.h"

#include "../../../common/simd/simd.h"

  /*! Ray structure. Contains all information about a ray including
   *  precomputed reciprocal direction. */
  struct Ray
  {
    /*! Default construction does nothing. */
    __forceinline Ray() {}

    /*! Constructs a ray from origin, direction, and ray segment. Near
     *  has to be smaller than far. */
    __forceinline Ray(const embree::Vec3fa& org, const embree::Vec3fa& dir, 
			 float tnear = embree::zero, float tfar = embree::inf, 
			 float time = embree::zero, int mask = -1)
      : org(org), tnear(tnear), dir(dir), tfar(tfar), time(time), mask(mask), geomID(-1), primID(-1), instID(-1)  {}

    /*! Tests if we hit something. */
    __forceinline operator bool() const { return geomID != RTC_INVALID_GEOMETRY_ID; }

  public:
    embree::Vec3f org;        //!< Ray origin
    float tnear;              //!< Start of ray segment
    embree::Vec3f dir;        //!< Ray direction
    float tfar;               //!< End of ray segment
    float time;               //!< Time of this ray for motion blur.
    unsigned int mask;        //!< used to mask out objects during traversal

  public:
    embree::Vec3f Ng;         //!< Not normalized geometry normal
    float u;                  //!< Barycentric u coordinate of hit
    float v;                  //!< Barycentric v coordinate of hit
    unsigned int geomID;           //!< geometry ID
    unsigned int primID;           //!< primitive ID
    unsigned int instID;           //!< instance ID

    // ray extensions
  public:
    RTCFilterFunctionN filter;
    embree::Vec3fa transparency;
  };

typedef Ray Ray1;

__forceinline RTCRay* RTCRay_(Ray& ray) {
  return (RTCRay*)&ray;
}

  /*! Outputs ray to stream. */ 
  inline std::ostream& operator<<(std::ostream& cout, const Ray& ray) {
    return cout << "{ " << 
      "org = " << ray.org << ", dir = " << ray.dir << ", near = " << ray.tnear << ", far = " << ray.tfar << ", time = " << ray.time << ", " <<
      "instID = " << ray.instID <<  ", geomID = " << ray.geomID << ", primID = " << ray.primID <<  ", " << "u = " << ray.u <<  ", v = " << ray.v << ", Ng = " << ray.Ng << " }";
}
