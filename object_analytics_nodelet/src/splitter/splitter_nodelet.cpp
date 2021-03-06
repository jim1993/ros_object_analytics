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

#include <pluginlib/class_list_macros.h>

#include "object_analytics_nodelet/splitter/splitter_nodelet.h"

namespace object_analytics_nodelet
{
namespace splitter
{
void SplitterNodelet::onInit()
{
  ros::NodeHandle nh = getNodeHandle();
  impl_.reset(new Splitter(nh));
}
}  // namespace splitter
}  // namespace object_analytics_nodelet
PLUGINLIB_EXPORT_CLASS(object_analytics_nodelet::splitter::SplitterNodelet, nodelet::Nodelet)
