#include "rclcpp/rclcpp.hpp"    //“”优先在你当前项目的源码目录中查找
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"   //因为源程序中使用了Uint32类型
#include "village_interfaces/srv/sell_novel.hpp"
#include <chrono>               //允许使用诸如100ms、1s、1min等时间单位


using std::placeholders::_1;
using std::placeholders::_2;

class PoorManNode:public rclcpp::Node // 创建一个类节点，继承自Node公有成员
{
    public:
        PoorManNode(std::string name):Node(name)
        {
            RCLCPP_INFO(this->get_logger(),"大家好，我是穷鬼%s，但我有买书的%d钱",name.c_str(), moneyForNovel);
            timer_ = this->create_wall_timer(std::chrono::seconds(timer_period), std::bind(&PoorManNode::timer_callback, this));
            //实例化服务接口的客户端，唯一的对应服务的名称sell_novel
            client_ = this->create_client<village_interfaces::srv::SellNovel>("sell_novel");
        }

        /*请求函数*/
        void buy_novel()
        {
            RCLCPP_INFO(this->get_logger(), "买小说去喽");
            //1.等待对应的服务端上线，查找服务维持1s,如果服务启动了，就跳出循环，否则就一直循环下去
            while (!client_->wait_for_service(std::chrono::seconds(1)))
            {
                //等待时检测rclcpp的状态
                if (!rclcpp::ok())
                {
                    RCLCPP_ERROR(this->get_logger(), "等待服务的过程中被打断...");
                    return;
                }
                RCLCPP_INFO(this->get_logger(), "等待服务端上线中");
            }
            
            if(moneyForNovel <= 0)
            {
                RCLCPP_INFO(this->get_logger(), "穷鬼%s没钱了，买不了小说了",this->get_name());
                return;
            }
            else
            {
                RCLCPP_INFO(this->get_logger(), "穷鬼%s还剩%d钱，又来买小说了",this->get_name(), moneyForNovel);
                //2.构造智能指针类型的请求的钱，如果只是声明则用SharedPtr
                auto request = std::make_shared<village_interfaces::srv::SellNovel_Request>();
                //请求消息中输入购书金额每次不超过3元
                request->money = (moneyForNovel > 3 ? 3 : moneyForNovel); 
                moneyForNovel = (moneyForNovel > 3 ? moneyForNovel-3 : moneyForNovel);
                
                //3.发送异步请求，然后等待返回，返回时调用回调函数。
                //异步机制：这里没有使用同步等待（即没有死等结果），而是发出请求后立刻返回，不阻塞主线程。
                //将当前类的 novels_callback 方法与当前对象(this)绑定，_1 代表将来传入的响应参数
                client_->async_send_request(request,std::bind(&PoorManNode::novels_callback, this, _1));
            }    
        }



    private:
        // 创建一个客户端成员对象
        rclcpp::Client<village_interfaces::srv::SellNovel>::SharedPtr client_;
        
        //定时5秒购买小说章节
        uint32_t timer_period = 5;
        rclcpp::TimerBase::SharedPtr timer_;
        
        //用于购买小说章节的存款
        uint32_t moneyForNovel = 50;
        
        //接收小说-回调函数，第一个参数response是一个Future类对象(对应异步请求的参数的占位符_1)，包含了异步请求后服务返回的结果
        void novels_callback(rclcpp::Client<village_interfaces::srv::SellNovel>::SharedFuture  response)
        {
            //等价于village_interfaces::srv::SellNovel::Response::SharedPtr
            //get()方法提取future对象包含的内部消息(实际上是一个智能指针)
            auto result = response.get();   
        
            RCLCPP_INFO(this->get_logger(), "收到%ld章的小说，现在开始按章节开读", result->novels.size());
        
            if(result->novels.size() == 0)
            {
                RCLCPP_INFO(this->get_logger(), "穷鬼%s买书失败，书库里没有小说章节了",this->get_name());
                moneyForNovel += 3;  //买书失败，退回钱
                return;
            }
            //novel为遍历用的临时变量，后面接容器(数组、向量、队列等)
            for(std::string novel:result->novels)
            {
                //打印小说章节内容
                RCLCPP_INFO(this->get_logger(), "%s", novel.c_str());
            }
            
            RCLCPP_INFO(this->get_logger(), "小说读完了，好刺激，写的真不错，好期待下面的章节呀！");
            
        }

        void timer_callback()
        {
            RCLCPP_INFO(this->get_logger(), "定时器回调函数被调用了,时间间隔设定为5s");
            buy_novel();
        }

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    /*产生一个Zhang4的节点*/
    auto node = std::make_shared<PoorManNode>("zhang4");
    /* 运行节点，并检测rclcpp状态*/
//    node->buy_novel();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}