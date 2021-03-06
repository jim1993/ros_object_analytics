/*
 * Copyright (c) 2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>

#include "object_analytics_nodelet/const.h"

namespace object_analytics_nodelet
{
const std::string Const::kTopicRegisteredPC2 = "/camera/depth_registered/points";
const std::string Const::kTopicPC2 = "pointcloud";
const std::string Const::kTopicRgb = "rgb";
const std::string Const::kTopicSegmentation = "segmentation";
const std::string Const::kTopicDetection = "detection";
const std::string Const::kTopicLocalization = "localization";
const std::string Const::kTopicTracking = "tracking";
const std::string Const::kTopicColorCameraInfo = "/camera/color/camera_info";
const std::string Const::kTopicDepthCameraInfo = "/camera/depth/camera_info";

const int Const::kTimeoutCameraInfo = 30;
}  // namespace object_analytics_nodelet
