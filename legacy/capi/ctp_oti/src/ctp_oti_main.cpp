/*
 * ctp_oti_main.cpp
 *
 *  Created on: May 12, 2014
 *      Author: dt
 */
#include "ATU_CTP_OTI.h"
#include "Toolbox.h"
class TEST_OUTPUT_Handler {
public:
	void printMsg(string msg)
	{
		cout << msg;
	}
};
class TEST_CTP_Executor {
public:
	TEST_CTP_Executor();
};
extern "C" ATU_Abstract_OTI *create_object() {
	ATU_Abstract_OTI *ctp_oti = new ATU_CTP_OTI();
	return ctp_oti;
}
extern "C" void destroy_object(ATU_Abstract_OTI *o) {
	delete o;
}
/*
int amain() {
//copy libthosttraderapi.so in the Debug directory
//run the ctp_oti in the Debug directory
	cout << "export LD_LIBRARY_PATH=../lib/ctp" << endl;
	cout <<	"run the ctp_oti in the Debug directory" << endl;

	TEST_OUTPUT_Handler *test_output_handler=new TEST_OUTPUT_Handler();
	ATU_CTP_OTI *ctp_oti = new ATU_CTP_OTI();
	ctp_oti->detach();
	usleep(1000*1000);
	ATU_TCP_OTI_string_handler *handler = new ATU_TCP_OTI_string_handler();
	handler->register_signalfeed_call_back_func(new ATU_OTI_signalfeed_CallBackFunc(boost::bind(&ATU_CTP_OTI::on_process_signalfeed, ctp_oti, _1)));
	ctp_oti->register_notify_orderfeed_call_back_func(new ATU_OTI_orderfeed_CallBackFunc(boost::bind(&ATU_TCP_OTI_string_handler::send_orderfeed, handler, _1)));
	ctp_oti->register_notify_tradefeed_call_back_func(new ATU_OTI_tradefeed_CallBackFunc(boost::bind(&ATU_TCP_OTI_string_handler::send_tradefeed, handler, _1)));
	handler->register_broadcast_msg_call_back_func(new ATU_String_CallBackFunc(boost::bind(&TEST_OUTPUT_Handler::printMsg,test_output_handler,_1) ) );
	handler->register_send_msg_call_back_func(new ATU_String_CallBackFunc(boost::bind(&TEST_OUTPUT_Handler::printMsg,test_output_handler,_1) ) );
	handler->register_portfolio_get_working_orders_call_back_func((new ATU_OTI_portfolio_get_working_orders_CallBackFunc(boost::bind(&ATU_CTP_OTI::on_process_portfolio_get_working_orders, ctp_oti, _1))));
	handler->register_portfolio_get_trade_history_call_back_func((new ATU_OTI_portfolio_get_trade_history_CallBackFunc(boost::bind(&ATU_CTP_OTI::on_process_portfolio_get_trade_history, ctp_oti, _1))));

	string testmsg;
	testmsg="20140422_111301_101101,signalfeed,CFFEX,IF1405,EATEST_00505,2167,1,open,1,delete,stop_order,today,stop_price=2166;stop_condition=3;stop_order_type=limit_order";
	testmsg="20140122_113451_123213,portfolio_get_working_orders,today";
	while(true) {
//TODO:: please check if this is correct or not....
		cin >> testmsg;
//===========================================================
		handler->parse_and_handle_msg(testmsg);
	}

//	usleep(1000*1000*10);
}
*/
