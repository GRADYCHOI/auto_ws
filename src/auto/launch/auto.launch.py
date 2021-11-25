from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='auto',
            executable='sense',
        ),
        Node(
            package='auto',
            executable='compute',
        ),
        Node(
            package='auto',
            executable='actuate',
        ),
    ])

