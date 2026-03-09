/*
 * my_world_plugin.cpp
 * ====================
 * Contoh World Plugin Gazebo Classic untuk Percobaan 10 (Advanced)
 * ROS 2 Humble – menggunakan gazebo_ros::Node dan rclcpp
 *
 * Plugin ini:
 *   1. Menerima event Update setiap langkah simulasi
 *   2. Mempublikasikan waktu simulasi ke topik /plugin/sim_time
 *   3. Mempublikasikan status setiap 2 detik simulasi ke /plugin/status
 *
 * Cara membangun:
 *   colcon build --packages-select gazebo_praktikum
 *
 * Cara menggunakan (dalam file .world):
 *   <plugin name="my_world_plugin" filename="libmy_world_plugin.so"/>
 */

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>

// gazebo_ros menyediakan node ROS 2 yang terintegrasi dengan lifecycle Gazebo
#include <gazebo_ros/node.hpp>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/string.hpp>

#include <memory>
#include <string>

namespace gazebo
{

class MyWorldPlugin : public WorldPlugin
{
public:
  MyWorldPlugin() : WorldPlugin()
  {}

  // Dipanggil saat plugin dimuat ke world
  void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf) override
  {
    world_ = _world;

    // Dapatkan node ROS 2 yang dikelola oleh gazebo_ros
    ros_node_ = gazebo_ros::Node::Get(_sdf);

    RCLCPP_INFO(ros_node_->get_logger(),
                "[MyWorldPlugin] Dimuat ke world: %s",
                world_->Name().c_str());

    // Publisher
    pub_sim_time_ = ros_node_->create_publisher<std_msgs::msg::Float64>(
        "/plugin/sim_time", rclcpp::QoS(10));
    pub_status_ = ros_node_->create_publisher<std_msgs::msg::String>(
        "/plugin/status", rclcpp::QoS(10));

    // Daftarkan callback Update (dipanggil setiap physics step)
    update_conn_ = event::Events::ConnectWorldUpdateBegin(
        std::bind(&MyWorldPlugin::OnUpdate, this));

    last_log_time_ = world_->SimTime().Double();

    RCLCPP_INFO(ros_node_->get_logger(),
                "[MyWorldPlugin] Siap. Publish ke /plugin/sim_time dan /plugin/status");
  }

  // Dipanggil setiap langkah simulasi
  void OnUpdate()
  {
    double sim_time = world_->SimTime().Double();

    // Publish waktu simulasi
    auto msg_time = std_msgs::msg::Float64();
    msg_time.data = sim_time;
    pub_sim_time_->publish(msg_time);

    // Log dan publish status setiap 2 detik
    if (sim_time - last_log_time_ >= 2.0)
    {
      last_log_time_ = sim_time;

      uint32_t model_count = world_->ModelCount();
      std::string status_str =
          "[MyWorldPlugin] SimTime=" + std::to_string(static_cast<int>(sim_time)) +
          "s, Models=" + std::to_string(model_count);

      RCLCPP_INFO(ros_node_->get_logger(), "%s", status_str.c_str());

      auto msg_status = std_msgs::msg::String();
      msg_status.data = status_str;
      pub_status_->publish(msg_status);
    }

    // Proses callback ROS 2 secara non-blocking
    rclcpp::spin_some(ros_node_);
  }

private:
  physics::WorldPtr    world_;
  event::ConnectionPtr update_conn_;

  gazebo_ros::Node::SharedPtr ros_node_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pub_sim_time_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr  pub_status_;

  double last_log_time_{0.0};
};

// Daftarkan plugin ke Gazebo
GZ_REGISTER_WORLD_PLUGIN(MyWorldPlugin)

}  // namespace gazebo
