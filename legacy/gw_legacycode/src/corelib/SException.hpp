#ifndef SEXCEPTION_HPP
#define SEXCEPTION_HPP

#include <exception>
#include <iostream>
using namespace std;

class SException : public std::exception
{
	public:
		void PrintMsg(const char* c1, const char* c2, const int i, const char* c3)
		{
			std::cout << "\t\t=========================================" << endl;
			std::cout << "\t\tSException thrown: File=";
			std::cout << c1 << ": Function=";
			std::cout << c2 << ": Line=";
			std::cout << i << ". ";
			std::cout << "MoreInfo: " << c3 << endl;
			std::cout << "\t\t=========================================" << endl;
		}
		void PrintMsg(const char* c1, const char* c2, const int i)
		{
			PrintMsg(c1,c2,i,"Nil");
		}
		virtual const char* what() const throw()
		{
			return "SException.";
		}
};

#endif
