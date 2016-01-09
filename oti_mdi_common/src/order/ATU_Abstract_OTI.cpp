///WARNING INCOMPLETED IMPLEMENTATION
#include "ATU_Abstract_OTI.h"

using namespace std;
namespace atu
{



ATU_Abstract_OTI::ATU_Abstract_OTI() :
		m_signalfeed_call_back_func(NULL),
		m_convert_signalfeed_call_back_func(NULL),
		m_portfolio_get_working_orders_call_back_func(NULL),
		m_logfeed_call_back_func(NULL),
		m_portfolio_get_trade_history_call_back_func(NULL),
		m_portfolio_get_PnL_call_back_func(NULL),
		m_process_tick_call_back_func(NULL)
{
			m_notify_tradefeed_call_back_func_list.clear();
			m_notify_orderfeed_call_back_func_list.clear();
			m_notify_errorfeed_call_back_func_list.clear();
}
ATU_Abstract_OTI::~ATU_Abstract_OTI() {

}
//would then call all the notify_tradefeed for all the order in the list
bool ATU_Abstract_OTI::on_notify_tradefeed(ATU_OTI_tradefeed_struct &s) {
	return true;
}
bool ATU_Abstract_OTI::on_notify_errorfeed(ATU_ErrorMsgStruct *ems) {
	return true;
}
bool ATU_Abstract_OTI::on_notify_orderfeed(ATU_OTI_orderfeed_struct &s) {
	return true;
}
bool ATU_Abstract_OTI::on_notify_riskfeed(ATU_OTI_riskfeed_struct &s) {
	return true;
}
bool ATU_Abstract_OTI::on_notify_riskstatusfeed(ATU_OTI_riskstatusfeed_struct &s) {
	return true;
}
//process incoming signal feed and dispatch for all the order in the list if necessary
bool ATU_Abstract_OTI::on_process_signalfeed(ATU_OTI_signalfeed_struct &s) {
//here we determine what order to create
	return true;
}
bool ATU_Abstract_OTI::on_process_quoterequestfeed(ATU_OTI_quoterequestfeed_struct &s) {
//here we determine what order to create
	return true;
}
bool ATU_Abstract_OTI::on_notify_portfoliofeed(ATU_OTI_portfoliofeed_struct &s){
	return true;
}

bool ATU_Abstract_OTI::on_process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &s) {
	return true;
}
bool ATU_Abstract_OTI::process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &s) {
	return true;
}

bool ATU_Abstract_OTI::process_portfolio_get_PnL(ATU_OTI_portfolio_get_PnL_struct &s) {
	return true;
}

bool ATU_Abstract_OTI::process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &s)
{
	return true;
}
bool ATU_Abstract_OTI::on_process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &s)
{
	return true;
}
bool ATU_Abstract_OTI::on_process_portfolio_get_PnL(ATU_OTI_portfolio_get_PnL_struct &s)
{
	return true;
}
//register send order callback func
void ATU_Abstract_OTI::register_signalfeed_call_back_func(ATU_OTI_signalfeed_CallBackFunc *callback) {
	m_signalfeed_call_back_func=callback;
}
//register convert order callback func
void ATU_Abstract_OTI::register_convert_signalfeed_call_back_func(ATU_OTI_signalfeed_CallBackFunc *callback) {
	m_convert_signalfeed_call_back_func=callback;
}

void ATU_Abstract_OTI::register_portfolio_get_working_orders_call_back_func(ATU_OTI_portfolio_get_working_orders_CallBackFunc *callback)
{
	m_portfolio_get_working_orders_call_back_func=callback;
}
void ATU_Abstract_OTI::register_portfolio_get_trade_history_call_back_func(ATU_OTI_portfolio_get_trade_history_CallBackFunc *callback)
{
	m_portfolio_get_trade_history_call_back_func=callback;
}
void ATU_Abstract_OTI::register_portfolio_get_PnL_call_back_func(ATU_OTI_portfolio_get_PnL_CallBackFunc *callback){
	m_portfolio_get_PnL_call_back_func=callback;
}
void ATU_Abstract_OTI::register_notify_logfeed_call_back_func(ATU_logfeed_CallBackFunc *callback)
{
	m_logfeed_call_back_func=callback;
}

bool ATU_Abstract_OTI::register_notify_orderfeed_call_back_func(ATU_OTI_orderfeed_CallBackFunc *callback)
{
	m_notify_orderfeed_call_back_func_list.push_back(callback);
	return true;
}
bool ATU_Abstract_OTI::register_notify_tradefeed_call_back_func(ATU_OTI_tradefeed_CallBackFunc *callback)
{
	m_notify_tradefeed_call_back_func_list.push_back(callback);
	return true;
}
bool ATU_Abstract_OTI::register_notify_errorfeed_call_back_func(ATU_errorfeed_CallBackFunc *callback)
{
	m_notify_errorfeed_call_back_func_list.push_back(callback);
	return true;
}
bool ATU_Abstract_OTI::register_notify_riskfeed_call_back_func(ATU_OTI_riskfeed_CallBackFunc *callback)
{
	m_notify_riskfeed_call_back_func_list.push_back(callback);
	return true;
}
bool ATU_Abstract_OTI::register_notify_riskstatusfeed_call_back_func(ATU_OTI_riskstatusfeed_CallBackFunc *callback)
{
	m_notify_riskstatusfeed_call_back_func_list.push_back(callback);
	return true;
}

bool ATU_Abstract_OTI::register_notify_portfoliofeed_call_back_func(ATU_OTI_portfoliofeed_CallBackFunc *callback){
	m_notify_portfoliofeed_call_back_func_list.push_back(callback);
	return true;
}
bool ATU_Abstract_OTI::notify_logfeed(ATU_logfeed_struct *s)
{
	if (m_logfeed_call_back_func!=NULL) {
		(*m_logfeed_call_back_func)(s);
	}
	return true;
}
//process orderfeed from lower layer and handle it accordingly
bool ATU_Abstract_OTI::notify_orderfeed(ATU_OTI_orderfeed_struct &s)
{
	for(unsigned int i = 0 ; i < m_notify_orderfeed_call_back_func_list.size() ; i++){
		if (m_notify_orderfeed_call_back_func_list[i]!=NULL) {
			 (*m_notify_orderfeed_call_back_func_list[i])(s);
		} else {
			return false;
		}
	}
	return true;
}
//process tradefeed from lower layer and handle it accordingly
bool ATU_Abstract_OTI::notify_tradefeed(ATU_OTI_tradefeed_struct &s)
{
	for(unsigned int i = 0 ; i < m_notify_tradefeed_call_back_func_list.size() ; i++){
		if (m_notify_tradefeed_call_back_func_list[i]!=NULL) {
			 (*m_notify_tradefeed_call_back_func_list[i])(s);
		} else {
			return false;
		}
	}
	return true;
}
bool ATU_Abstract_OTI::notify_errorfeed(ATU_ErrorMsgStruct *ems)
{
	for(unsigned int i = 0 ; i < m_notify_errorfeed_call_back_func_list.size() ; i++){
		if (m_notify_errorfeed_call_back_func_list[i]!=NULL) {
			 (*m_notify_errorfeed_call_back_func_list[i])(ems);
		} else {
			return false;
		}
	}
	return true;
}
bool ATU_Abstract_OTI::notify_riskfeed(ATU_OTI_riskfeed_struct &s)
{
//	cout << "ATU_Abstract_OTI::notify_riskfeed" << endl;
	for(unsigned int i = 0 ; i < m_notify_riskfeed_call_back_func_list.size() ; i++){
		if (m_notify_riskfeed_call_back_func_list[i]!=NULL) {
//			cout << "ATU_Abstract_OTI::notify_riskfeed before calling callback" << endl;
			 (*m_notify_riskfeed_call_back_func_list[i])(s);
		} else {
			return false;
		}
	}
	return true;
}
bool ATU_Abstract_OTI::notify_riskstatusfeed(ATU_OTI_riskstatusfeed_struct &s)
{
	for(unsigned int i = 0 ; i < m_notify_riskstatusfeed_call_back_func_list.size() ; i++){
		if (m_notify_riskstatusfeed_call_back_func_list[i]!=NULL) {
			 (*m_notify_riskstatusfeed_call_back_func_list[i])(s);
		} else {
			return false;
		}
	}
	return true;
}

bool ATU_Abstract_OTI::notify_portfoliofeed(ATU_OTI_portfoliofeed_struct &s)
{
	for(unsigned int i = 0 ; i < m_notify_portfoliofeed_call_back_func_list.size() ; i++){
		if (m_notify_portfoliofeed_call_back_func_list[i]!=NULL) {
			 (*m_notify_portfoliofeed_call_back_func_list[i])(s);
		} else {
			return false;
		}
	}
	return true;
}



bool ATU_Abstract_OTI::on_process_marketfeed(ATU_MDI_marketfeed_struct &s)
{
  return true;
}
bool ATU_Abstract_OTI::on_process_ohlcfeed(ATU_MDI_ohlcfeed_struct &s)
{
  return true;
}

bool ATU_Abstract_OTI::register_process_tick_call_back_func(ATU_OTI_Tick_CallBackFunc * callback){
	m_process_tick_call_back_func = callback;
	return true;
}


}
