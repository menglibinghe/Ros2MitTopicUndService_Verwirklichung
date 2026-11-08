
import rclpy
from rclpy.node import Node
from std_msgs.msg import String,UInt32

class BaipiaoNode(Node):
    """
    创建一个白嫖节点，初始化时输出一句话
    """
    def __init__(self,name):
        super().__init__(name)
        self.get_logger().info(f"大家好，我是{name},我是一名白嫖党！")
        # 创建并初始化发布者成员属性pubnovel         

        # 创建并初始化订阅者成员属性submoney,消息类型是无符号32位
        self.sub_novel = self.create_subscription(String,
            "sexy_girl",self.subscriptCallback,10)

    def subscriptCallback(self, novel):
        """
        4. 编写订阅回调处理逻辑
        """
        self.get_logger().info(f"I have read:  {novel.data}")

        
def main(args=None):
    """
    ros2运行该节点的入口函数
    编写ROS2节点的一般步骤
    1. 导入库文件
    2. 初始化客户端库
    3. 新建节点对象
    4. spin循环节点
    5. 关闭客户端库
    """
    rclpy.init(args=args) # 初始化rclpy
    node = BaipiaoNode("li3")  # 新建一个节点
    node.get_logger().info(f"大家好,我是{node.get_name()},李四他哥，我可以白嫖李四的小说！")
    rclpy.spin(node) # 保持节点运行，检测是否收到退出指令（Ctrl+C）
    rclpy.shutdown() # 关闭rclpy