// Intrepreter 1.cpp 
// Levi Seibert
// CS 414
// This program is an interpreter that reads in generation 0 programming statements and executes them
// by translating them into C++ statements

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
using namespace std;

//open input file
ifstream inputFile("Program 1.txt");

//data section structure
struct dat
{
	char sign = ' ';
	int dig1 = 0;
	int dig2 = 0;
	int dig3 = 0;
	int dig4 = 0;
};

//program section structure
struct pro
{
	char sign = ' ';
	int operation = 0;
	int oper1 = 0;
	int oper2 = 0;
	int destination = 0;
};

//get the data from the file and store in data and program arrays
void extractData(dat data[], pro program[])
{	
	int i = 0;
	do 
	{
		inputFile >> data[i].sign;
		inputFile >> data[i].dig1;
		inputFile >> data[i].dig2;
		inputFile >> data[i].dig3;
		inputFile >> data[i].dig4;
		i++;
	} while (data[i - 1].dig1 != 9);

	i = 0;
	do
	{
		inputFile >> program[i].sign;
		inputFile >> program[i].operation;
		inputFile >> program[i].oper1;
		inputFile >> program[i].oper2;
		inputFile >> program[i].destination;
		i++;
	} while (program[i - 1].operation != 9);
}

int main()
{
	//define and initalize variables and arrays
	int ip = 0, cp = 0;
	int dest, sub, subscript, num, end, dividend;
	dat data[999]; 
	pro program[999];

	//call extractData function to fill the arrays
	extractData(data, program);
	
	do
	{	
		long long int number;

		//move the current pointer
		cp = ip;

		//increment the instruction pointer
		ip++;

		if (program[cp].sign == '+')
		{
			switch (program[cp].operation)
			{
			//+1 Add
			case 1: 
				dest = program[cp].destination;
				data[dest].dig1 = data[program[cp].oper1].dig1 + data[program[cp].oper2].dig1;
				data[dest].dig2 = data[program[cp].oper1].dig2 + data[program[cp].oper2].dig2;
				data[dest].dig3 = data[program[cp].oper1].dig3 + data[program[cp].oper2].dig3;
				data[dest].dig4 = data[program[cp].oper1].dig4 + data[program[cp].oper2].dig4;
				
				//take care of carries
				while (data[dest].dig4 > 999)
				{
					data[dest].dig3++;
					data[dest].dig4 -= 1000;
				}
				while (data[dest].dig3 > 999)
				{
					data[dest].dig2++;
					data[dest].dig3 -= 1000;
				}
				while (data[dest].dig2 > 999)
				{
					data[dest].dig1++;
					data[dest].dig2 -= 1000;
				}
				break;
			//+2 Multiply
			case 2:
				dest = program[cp].destination;
				data[dest].dig4 = data[program[cp].oper1].dig4 * data[program[cp].oper2].dig4;
				while (data[dest].dig4 > 999)
				{
					data[dest].dig3++;
					data[dest].dig4 -= 1000;
				}
				while (data[dest].dig3 > 999)
				{
					data[dest].dig2++;
					data[dest].dig3 -= 1000;
				}
				while (data[dest].dig2 > 999)
				{
					data[dest].dig1++;
					data[dest].dig2 -= 1000;
				}
				break;
			//+3 Square
			case 3:
				dest = program[cp].destination;
				data[dest].dig4 = data[program[cp].oper1].dig4 * data[program[cp].oper1].dig4;
				break;
			//+4 =
			case 4:
				if (data[program[cp].oper1].dig4 == data[program[cp].oper2].dig4)
					ip = program[cp].destination;
				break;
			//+5 >=
			case 5:
				if (data[program[cp].oper1].dig4 >= data[program[cp].oper2].dig4)
					ip = program[cp].destination;
				break;
			//+6 x[i]->z
			case 6:
				dest = program[cp].destination;
				sub = program[cp].oper2;
				subscript = program[cp].oper1 + data[sub].dig4;
				data[dest].dig4 = data[subscript].dig4;
				break;
			//+7 (iii nnn ddd) Increment iii and loops to instruction ddd if the result is less than the contents of nnn.
			case 7:
				program[cp].oper1++;
				end = data[program[cp].oper2].dig4;
				if (program[cp].oper1 < end)
					ip = program[cp].destination;
				break;
			//+8 Input
			case 8:
				inputFile >> number;
				dest = program[cp].destination;
				data[dest].dig1 = number / 1000000000;
				number = number % 1000000000;
				data[dest].dig2 = number / 1000000;
				number = number % 1000000;
				data[dest].dig3 = number / 1000;
				number = number % 1000;
				data[dest].dig4 = number;
				break;
			//+9 End
			case 9:
				break;
			}
		}

		else
		{
			switch (program[cp].operation)
			{
			//-1 Subtract
			case 1:
				dest = program[cp].destination;
				data[dest].dig1 = data[program[cp].oper1].dig1 - data[program[cp].oper2].dig1;
				data[dest].dig2 = data[program[cp].oper1].dig2 - data[program[cp].oper2].dig2;
				data[dest].dig3 = data[program[cp].oper1].dig3 - data[program[cp].oper2].dig3;
				data[dest].dig4 = data[program[cp].oper1].dig4 - data[program[cp].oper2].dig4;

				//take care of carries
				while (data[dest].dig4 < 0)
				{
					data[dest].dig3--;
					data[dest].dig4 += 1000;
				}
				while (data[dest].dig3 < 0)
				{
					data[dest].dig2--;
					data[dest].dig3 += 1000;
				}
				while (data[dest].dig2 < 0)
				{
					data[dest].dig1--;
					data[dest].dig2 += 1000;
				}
				break;
			//-2 Divide
			case 2:
				dest = program[cp].destination;
				dividend = data[program[cp].oper1].dig1 * 1000000000 + data[program[cp].oper1].dig2 * 1000000
					+ data[program[cp].oper1].dig3 * 1000 + data[program[cp].oper1].dig4;
				data[dest].dig4 = dividend / data[program[cp].oper2].dig4;
				 while (data[dest].dig4 > 999)
				{
					data[dest].dig3++;
					data[dest].dig4 -= 1000;
				}
				while (data[dest].dig3 > 999)
				{
					data[dest].dig2++;
					data[dest].dig3 -= 1000;
				}
				while (data[dest].dig2 > 999)
				{
					data[dest].dig1++;
					data[dest].dig2 -= 1000;
				}
				break;
			//-3 Square Root
			case 3:
				dest = program[cp].destination;
				data[dest].dig4 = sqrt(data[program[cp].oper1].dig4);
				break;
			//-4 !=
			case 4:
				if (data[program[cp].oper1].dig4 != data[program[cp].oper2].dig4) 
					ip = program[cp].destination;
				break;
			//-5 <
			case 5:
				if (data[program[cp].oper1].dig4 < data[program[cp].oper2].dig4)
					ip = program[cp].destination;
				break;
			//-6 x->y[i]
			case 6:
				sub = program[cp].destination;
				subscript = program[cp].oper2 + data[sub].dig4;
				dest = program[cp].oper1;
				data[subscript].dig4 = data[dest].dig4;
				break;
			//-7 End
			case 7:
				break;
			//-8 Output
			case 8:
				dest = program[cp].destination;
				cout << data[dest].dig1 << setw(3) << setfill('0') << data[dest].dig2 << setw(3) << setfill('0') << data[dest].dig3 << setw(3) << setfill('0') << data[dest].dig4 << endl << endl;
				break;
			//-9 End
			case 9:
				break;
			}
		}

	//continue until the operation is 9
	} while (program[cp].operation != 9);

	//close the input file
	inputFile.close();

	return 0;
}