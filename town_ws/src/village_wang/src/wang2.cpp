#include "rclcpp/rclcpp.hpp"    //“”优先在你当前项目的源码目录中查找
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include "village_interfaces/srv/sell_novel.hpp"
#include <queue>            //<>表示直接去系统的标准库中查找

using std::placeholders::_1;
using std::placeholders::_2;

class SingleDogNode:public rclcpp::Node // 创建一个类节点，继承自Node公有成员
{
    public:
        SingleDogNode(std::string name):Node(name)
        {
            RCLCPP_INFO(this->get_logger(),"大家好，我是单身狗%s.",name.c_str());

            // 初始化一个订阅者来订阅李四写的小说，通过名字sexy_girl
            sub_novel = this->create_subscription<std_msgs::msg::String>(
                "sexy_girl", 10, std::bind(&SingleDogNode::topic_callback, this, _1));

            // 初始化一个发布者用于支付订阅李四的小说费用
            payed_fee = this->create_publisher<std_msgs::msg::UInt32>("sexy_girl_money", 10);   

        
            //实例化回调组对象，建立互斥回调组，保证回调函数执行期间共享数据比如图书章节不被其他回调修改
            callback_group_service_ = this->create_callback_group(
             rclcpp::CallbackGroupType::MutuallyExclusive);
            // 实例化卖二手书的服务成员对象，sell_novel是对应的服务名称
            server_ = this->create_service<village_interfaces::srv::SellNovel>(
            "sell_novel",std::bind(&SingleDogNode::sell_book_callback,this,_1,_2));
        }



    private:
        // 声明一个订阅者（成员变量，也是一个智能指针）,用于订阅小说
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_novel;
        
        // 声明一个发布者,由于支付订阅小说费用
        rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr payed_fee;

        
        // 收到话题接口的回调函数
        void topic_callback(const std_msgs::msg::String::SharedPtr msg)
        {
            
            RCLCPP_INFO_STREAM(this->get_logger(), "朕已阅：" << msg->data);
            
            std_msgs::msg::UInt32 money;
            money.data = 1;
            payed_fee->publish(money);
            RCLCPP_INFO_STREAM(this->get_logger(), "朕打赏了：" << money.data << " 元");
            
            //将小说放入novels_queue的末尾
            novels_queue.push(msg->data);
            
        }

        //创建一个小说章节队列（先进先出），用于存放接收到的小说章节
        std::queue<std::string>  novels_queue;    
        // 声明一个服务回调组对象，用于创建多线程程序
        rclcpp::CallbackGroup::SharedPtr callback_group_service_;
        // 声明一个服务端成员对象
        rclcpp::Service<village_interfaces::srv::SellNovel>::SharedPtr server_;
        //声明一个服务接口回调函数，当收到买书请求时自动调用该函数，用于处理数据
        //request是一个指向请求对象的智能指针(只读)。它包含了客户端发送过来的数据(即a和b)；
        //response是一个指向响应对象的智能指针(只读)，它们会接收服务端输入的数据，然后传递给客户端
        void sell_book_callback(const village_interfaces::srv::SellNovel::Request::SharedPtr request,
            const village_interfaces::srv::SellNovel::Response::SharedPtr response)
        {
            //对请求数据进行处理
            RCLCPP_INFO(this->get_logger(), "收到一个买书请求，一共给了%d钱",request->money);
            unsigned int novelsNum = request->money*1;  //应给小说数量，一块钱一章

            //判断当前书库里书的数量是否满足张三要买的数量，不够则进入等待函数
            if(novels_queue.size()<novelsNum)
            {
                RCLCPP_INFO(this->get_logger(), "当前艳娘传奇章节存量为%ld：不能满足需求,开始等待",novels_queue.size());

                // 设置loop_rate周期为1s
                rclcpp::Rate loop_rate(1);

                //当书库里小说数量小于请求数量时返回空数组
                if (novels_queue.size()<novelsNum)
                {
                    //打印一下当前的章节数量和缺少的数量
                    RCLCPP_INFO(this->get_logger(), "等待中，目前已有%ld章，还差%ld章",novels_queue.size(),novelsNum-novels_queue.size());
                    return;
                }
            }
            // 章节数量满足需求了
            RCLCPP_INFO(this->get_logger(), "当前艳娘传奇章节存量为%ld：已经满足需求",novels_queue.size());

            //把书取出来，放进请求响应对象response中
            for(unsigned int i =0 ;i<novelsNum;i++)
            {
                response->novels.push_back(novels_queue.front());   //取出队首的章节(把它们逐个添加到响应消息的数组末尾)，并发布出去
                novels_queue.pop();     //取出队首的章节字符串后，删除队首元素
            }
        }

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    /*产生一个Wang2的节点*/
    auto node = std::make_shared<SingleDogNode>("wang2"); 
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}