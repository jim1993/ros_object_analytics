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
#include <memory>
#include <vector>

#include <pcl_conversions/pcl_conversions.h>

#include <object_analytics_msgs/ObjectsInBoxes3D.h>
#include <object_analytics_msgs/ObjectInBox3D.h>

#include "object_analytics_nodelet/const.h"
#include "object_analytics_nodelet/model/projector_impl.h"
#include "object_analytics_nodelet/segmenter/organized_multi_plane_segmenter.h"
#include "object_analytics_nodelet/segmenter/segmenter.h"

namespace object_analytics_nodelet
{
namespace segmenter
{
using pcl::fromROSMsg;
using pcl::Label;
using pcl::Normal;
using pcl::PointIndices;
using pcl::IndicesPtr;
using pcl::copyPointCloud;
using object_analytics_nodelet::model::Object3D;
using object_analytics_nodelet::model::Projector;
using object_analytics_nodelet::model::ProjectorImpl;
using object_analytics_nodelet::segmenter::AlgorithmProvider;

Segmenter::Segmenter(ros::NodeHandle& nh) : provider_(new AlgorithmProvider(nh))
{
  sub_ = nh.subscribe(Const::kTopicPC2, 1, &Segmenter::cbSegment, this);
  pub_ = nh.advertise<object_analytics_msgs::ObjectsInBoxes3D>(Const::kTopicSegmentation, 1);
}

void Segmenter::cbSegment(const sensor_msgs::PointCloud2::ConstPtr& points)
{
  if (pub_.getNumSubscribers() == 0)
  {
    ROS_DEBUG_STREAM("No subscriber is listening on me, just skip");
    return;
  }

  PointCloudT::Ptr pointcloud(new PointCloudT);
  getPclPointCloud(points, *pointcloud);
  std::vector<Object3D> objects;
  segment(pointcloud, objects);
  publishResult(points->header, objects);
}

void Segmenter::getPclPointCloud(const sensor_msgs::PointCloud2::ConstPtr& points, PointCloudT& pcl_cloud)
{
  fromROSMsg<PointT>(*points, pcl_cloud);
}

void Segmenter::segment(const PointCloudT::ConstPtr& cloud, std::vector<Object3D>& objects)
{
  std::vector<PointIndices> cluster_indices;
  PointCloudT::Ptr cloud_segment(new PointCloudT);
  std::shared_ptr<Algorithm> seg = provider_->get();
  seg->segment(cloud, cloud_segment, cluster_indices);
  std::shared_ptr<Projector> projector = ProjectorImpl::instance();

  for (auto& indices : cluster_indices)
  {
    try
    {
      Object3D object3d(cloud_segment, indices.indices, projector);
      objects.push_back(object3d);
    }
    catch (std::exception& e)
    {
      ROS_ERROR_STREAM(e.what());
    }
  }

  ROS_DEBUG_STREAM("get " << objects.size() << " objects from segmentation");
}

void Segmenter::publishResult(const std_msgs::Header& header, const std::vector<Object3D>& objects)
{
  boost::shared_ptr<object_analytics_msgs::ObjectsInBoxes3D> msg =
      boost::make_shared<object_analytics_msgs::ObjectsInBoxes3D>();
  msg->header = header;

  for (auto& obj : objects)
  {
    object_analytics_msgs::ObjectInBox3D oib3;
    oib3.min = obj.getMin();
    oib3.max = obj.getMax();
    oib3.roi = obj.getRoi();
    msg->objects_in_boxes.push_back(oib3);
  }

  ROS_DEBUG_STREAM("segmenter publish message with " << objects.size() << " objects");
  pub_.publish(msg);
}
}  // namespace segmenter
}  // namespace object_analytics_nodelet
