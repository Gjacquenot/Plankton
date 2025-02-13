// Copyright (c) 2020 The Plankton Authors.
// All rights reserved.
//
// This source code is derived from UUV Simulator
// (https://github.com/uuvsimulator/uuv_simulator)
// Copyright (c) 2016-2019 The UUV Simulator Authors
// licensed under the Apache license, Version 2.0
// cf. 3rd-party-licenses.txt file in the root directory of this source tree.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __FIN_ROS_PLUGIN_HH__
#define __FIN_ROS_PLUGIN_HH__

#include <uuv_gazebo_plugins/FinPlugin.h>
#include <uuv_gazebo_ros_plugins_msgs/msg/float_stamped.hpp>
#include <uuv_gazebo_ros_plugins_msgs/srv/get_list_param.hpp>

#include <gazebo/common/Plugin.hh>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>

#include <map>
#include <memory>

namespace uuv_simulator_ros
{
  class FinROSPlugin : public gazebo::FinPlugin
  {
    using GetListParam = uuv_gazebo_ros_plugins_msgs::srv::GetListParam;
    using FloatStamped = uuv_gazebo_ros_plugins_msgs::msg::FloatStamped;

    /// \brief Constrcutor.
    public: FinROSPlugin();

    /// \brief Destructor.
    public: ~FinROSPlugin();

    /// \brief Load module and read parameters from SDF.
    public: void Load(gazebo::physics::ModelPtr _parent, sdf::ElementPtr _sdf);

    /// \brief Publish state via ROS.
    public: void RosPublishStates();

    /// \brief Set new set point.
    public: void SetReference(
        const uuv_gazebo_ros_plugins_msgs::msg::FloatStamped::SharedPtr _msg);

    /// \brief Return the list of paramaters of the lift and drag model
    public: void GetLiftDragParams(
      const GetListParam::Request::SharedPtr  _req,
            GetListParam::Response::SharedPtr _res);

    /// \brief Return the ROS publish period.
    public: gazebo::common::Time GetRosPublishPeriod();

    /// \brief Set the ROS publish frequency (Hz).
    public: void SetRosPublishRate(double _hz);

    /// \brief Initialize Module.
    public: virtual void Init();

    /// \brief Reset Module.
    public: virtual void Reset();

    /// \brief Pointer to this ROS node's handle.
    protected: std::shared_ptr<rclcpp::Node> myRosNode;

    /// \brief Subscriber reacting to new reference set points.
    private: rclcpp::Subscription<uuv_gazebo_ros_plugins_msgs::msg::FloatStamped>::SharedPtr mySubReference;

    /// \brief Publisher for current state.
    private: rclcpp::Publisher<uuv_gazebo_ros_plugins_msgs::msg::FloatStamped>::SharedPtr myPubState;

    /// \brief Publisher for current actual thrust.
    private: rclcpp::Publisher<geometry_msgs::msg::WrenchStamped>::SharedPtr myPubFinForce;

    /// \brief Connection for callbacks on update world.
    private: gazebo::event::ConnectionPtr rosPublishConnection;

    /// \brief Period after which we should publish a message via ROS.
    private: gazebo::common::Time rosPublishPeriod;

    /// \brief Map of services
    private: std::map<std::string,
                      rclcpp::Service<uuv_gazebo_ros_plugins_msgs::srv::GetListParam>::SharedPtr> myServicesById;

    /// \brief Last time we published a message via ROS.
    private: gazebo::common::Time lastRosPublishTime;
  };
}

#endif
