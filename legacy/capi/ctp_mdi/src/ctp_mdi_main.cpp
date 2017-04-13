/*
 * ctp_mdi_main.cpp
 *
 *  Created on: May 12, 2014
 *      Author: dt
 */
#include "ATU_Abstract_MDI.h"
#include "ATU_CTP_MDI.h"
using namespace atu;
class TEST_OUTPUT_Handler {
public:
	void printMsg(string msg)
	{
		cout << msg;
	}
};

extern "C" ATU_Abstract_MDI *create_object() {
	ATU_Abstract_MDI *ctp_mdi = new ATU_CTP_MDI();
	return ctp_mdi;
}
extern "C" void destroy_object(ATU_Abstract_MDI *o) {
	delete o;
}
/*
int main() {
//copy libthosttraderapi.so in the Debug directory
//run the ctp_mdi in the Debug directory
	cout << "export LD_LIBRARY_PATH=../lib/ctp" << endl;
	cout <<	"run the ctp_mdi in the Debug directory" << endl;

	TEST_OUTPUT_Handler *test_output_handler=new TEST_OUTPUT_Handler();
	ATU_CTP_MDI *ctp_mdi = new ATU_CTP_MDI();
	ctp_mdi->detach();
	usleep(1000*1000);
	ATU_TCP_MDI_string_handler *handler = new ATU_TCP_MDI_string_handler();
	handler->register_process_subscription_call_back_func(new ATU_MDI_subscription_CallBackFunc(boost::bind(&ATU_CTP_MDI::on_process_subscription, ctp_mdi, _1)));
	ctp_mdi->register_notify_marketfeed_call_back_func(new ATU_MDI_marketfeed_CallBackFunc(boost::bind(&ATU_TCP_MDI_string_handler::send_marketfeed, handler, _1)));
	handler->register_send_msg_call_back_func(new ATU_String_CallBackFunc(boost::bind(&TEST_OUTPUT_Handler::printMsg,test_output_handler,_1) ) );

	string testmsg;
	testmsg="20140422_111301_101101,signalfeed,CFFEX,IF1405,EATEST_00505,2167,1,open,1,delete,stop_order,today,stop_price=2166;stop_condition=3;stop_order_type=limit_order";
	testmsg="20140122_113451_123213,portfolio_get_working_orders,today";
	testmsg="20140122_113451_123213,subscription,CFFEX,IF1408,20140720,20140720";
//	while(true) {
//TODO:: please check if this is correct or not....
//		cin >> testmsg;
//===========================================================
		handler->parse_and_handle_msg(testmsg);
//	}
	sleep(100);
//	usleep(1000*1000*10);
}
*/
