import rclpy
from rclpy.node import Node
import std_msgs.msg

class PoorNode(Node):
    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info(f"我是穷鬼: {name}")
        self.sub_novel = self.create_subscription(std_msgs.msg.String,"sexy_girl",self.subscriptCallback,10)
        self.pubPay = self.create_publisher(std_msgs.msg.UInt32,"sexy_girl_money",10)
        self.i = 0
        self.timer = self.create_timer(3,self.timercallback)

    def subscriptCallback(self,msg):
        self.get_logger().info(f"I have read:  {msg.data}")
    
    def timercallback(self):
        self.get_logger().info(f"No.{self.i} contribution 10 yuanto li4)")
        fee = std_msgs.msg.UInt32()
        fee.data = 10
        self.pubPay.publish(fee)
        self.i += 1

def main(args=None):
    rclpy.init(args=args) # 初始化rclpy
    zhang3 = PoorNode("zhang3")
    rclpy.spin(zhang3) # 保持节点运行，检测是否收到退出指令（Ctrl+C）
    rclpy.shutdown() # 关闭rclpy