#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <list>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

const unsigned long maxNumber = 1000000000;

class bigNum
{
private:
    string longNumber;
    vector<unsigned long> number;
public:
    bigNum();
    bigNum(string number);
    void storeBigNumber();
    string getVal();
    void setVal(string s);
    vector<unsigned long> * getUnits();
    void add(bigNum x, bigNum y);
    bool sub(bigNum x, bigNum y);
    void mul(bigNum x, bigNum y);
    void div(bigNum x, bigNum y, bigNum &r);
    void ExtendedEUCLID(bigNum m);
};

int main(int argc, char** argv)
{
    //string p = "P=12369571528747655798110188786567180759626910465726920556567298659370399748072366507234899432827475865189642714067836207300153035059472237275816384410077871";
    //string q = "Q=2065420353441994803054315079370635087865508423962173447811880044936318158815802774220405304957787464676771309034463560633713497474362222775683960029689473";
    string p = "P=275816384410077871";
    string q = "Q=929689473";
    //string q = "Q=2";
    string e = "E=65537";
    //result = 296675817
    //remainder = 451503430
    p = p.substr(2);
    q = q.substr(2);
    e = e.substr(2);
    bigNum P(p);
    bigNum Q(q);
    bigNum RES("550");
    bigNum remainder;
    //bigNum RES;
    bigNum Phi("1759");
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    //P.sub(P, bigNum("1"));
    //Q.sub(Q, bigNum("1"));
    //Phi.mul(P, Q);
    RES.ExtendedEUCLID(Phi);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << duration << endl;
    cout << RES.getVal().length() << endl;
    cout << RES.getVal() << endl;
    //cout << remainder.getVal() << endl;
    return 0;
}

bigNum::bigNum()
{
    longNumber = "";
    number.clear();
}
bigNum::bigNum(string number)
{
    longNumber = number;
    storeBigNumber();
}
void bigNum::storeBigNumber()
{
    number.clear();
    unsigned int length = (unsigned int)longNumber.length();
    unsigned int remainder = length % 9;
    if(remainder > 0) number.push_back(stoul(longNumber.substr(0, remainder)));
    for(int i = remainder; i < length; i+=9)
    {
        number.push_back(stoul(longNumber.substr((unsigned long)i, 9)));
    }
}
string bigNum::getVal()
{
    return longNumber;
}
void bigNum::setVal(string s)
{
    longNumber = s;
}
vector<unsigned long> * bigNum::getUnits()
{
    return &number;
}
void bigNum::add(bigNum x, bigNum y)
{
    string tempStr;
    string testDigits;
    unsigned long long tempVal;
    vector<unsigned long> * numberX = x.getUnits();
    vector<unsigned long> * numberY = y.getUnits();
    while(numberX->size() > numberY->size()) numberY->insert(numberY->begin(), 0);
    while(numberY->size() > numberX->size()) numberX->insert(numberX->begin(), 0);
    int index = (int)numberX->size()-1;
    while(index >= 0)
    {
        tempVal = (*numberX)[index] + (*numberY)[index];
        if(tempVal > maxNumber-1)
        {
            tempVal %= maxNumber;
            if(index - 1 >= 0) (*numberX)[index - 1] += 1; //may be error if it's already 2^32 - 1
        }
        testDigits = to_string(tempVal);
        while(testDigits.length() < 9) testDigits.insert(0, "0");
        tempStr.insert(0, testDigits);
        index--;
    }
    longNumber = tempStr;
    storeBigNumber();
}
bool bigNum::sub(bigNum x, bigNum y)
{
    string tempStr;
    string testDigits;
    long long tempVal;
    bool negFlag = false;
    vector<unsigned long> * numberX = x.getUnits();
    vector<unsigned long> * numberY = y.getUnits();
    while(numberX->size() > numberY->size()) numberY->insert(numberY->begin(), 0);
    while(numberY->size() > numberX->size()) numberX->insert(numberX->begin(), 0);
    if(numberX[0] < numberY[0])
        negFlag = true;
    int index = (int)numberX->size()-1;
    while(index >= 0)
    {
        tempVal = (long long)((*numberX)[index] - (*numberY)[index]);
        if(tempVal < 0)
        {
            while(tempVal < 0) tempVal += maxNumber;
            if(index - 1 > 0) (*numberX)[index - 1] -= 1;
            else if(index - 1 == 0)
            {
                    if((*numberX)[0] > 0) (*numberX)[0] -= 1;
                else return false;
            }
            else return false;
        }
        testDigits = to_string(tempVal);
        while(testDigits.length() < 9) testDigits.insert(0, "0");
        tempStr.insert(0, testDigits);
        index--;
    }
    longNumber = tempStr;
    storeBigNumber();
    return true;
}
void bigNum::mul(bigNum x, bigNum y)
{
    string tempStr;
    string tempStrMain;
    string tempStrSec;
    unsigned long long offset = 0;
    unsigned int tempVal;
    vector<unsigned long> * numberX = x.getUnits();
    vector<unsigned long> * numberY = y.getUnits();
    int total_length = (int)(numberX->size() + numberY->size()) - 1;
    vector<unsigned long long> result((unsigned long)total_length, 0);
    for(int i = (int)numberX->size()-1; i >= 0; i--)
    {
        for(int j = (int)numberY->size()-1; j >= 0 ; j--)
        {
            result[(i+j)] += (*numberX)[i] * (*numberY)[j];
        }
    }
    for(int i = (int)result.size()-1; i >= 0; i--)
    {
        tempStr = to_string(result[i]);
        if(tempStr.length() > 9)
        {
            tempStrMain = tempStr.substr(tempStr.length() - 9, 9);
            tempStrSec = tempStr.substr(0, tempStr.length() - 9);
        }
        else
        {
            tempStrMain = tempStr;
            tempStrSec = "0";
        }
        result[i] = stoul(tempStrMain) + offset;
        if(result[i] > maxNumber-1)
        {
            tempVal = (unsigned int)(result[i] / maxNumber);
            result[i] %= maxNumber;
            offset = stoul(tempStrSec) + tempVal;
        }
        else
        {
            offset = stoul(tempStrSec);
        }
    }
    if(offset > 0)
    {
        result.insert(result.begin(), 0);
        result[0] = offset;
    }
    for(int i = 0; i < result.size(); i++)
    {
        tempStr = to_string(result[i]);
        while(tempStr.length() < 9) tempStr.insert(0, "0");
        longNumber.append(tempStr);
    }
    storeBigNumber();
}
void bigNum::div(bigNum x, bigNum y, bigNum &r)
{
    unsigned long result = 0;
    string RES = "";
    string xNumber = x.getVal();
    string yNumber = y.getVal();
    string newX = "";
    bool lFlag = false;
    bigNum temp;
    if(xNumber[0] > yNumber[0])
        lFlag = true;
    if(yNumber.length() > xNumber.length())
    {
        longNumber = "0";
        storeBigNumber();
        r = x;
        return;
    }
    if(yNumber == xNumber)
    {
        longNumber = "1";
        storeBigNumber();
        r.setVal("0");
        r.storeBigNumber();
        return;
    }
    if(lFlag) newX += xNumber.substr(0, yNumber.length());
    else newX += xNumber.substr(0, yNumber.length()+1);
    temp.setVal(newX);
    temp.storeBigNumber();
    for(int i = (int)newX.length(); i < xNumber.length(); i++)
    {
        while(temp.sub(temp, y))
            result++;
        RES.append(to_string(result));
        result = 0;
        temp.setVal(temp.getVal()+xNumber[i]);
        temp.storeBigNumber();
    }
    while(temp.sub(temp, y))
        result++;
    RES.append(to_string(result));
    longNumber = RES;
    storeBigNumber();
    r.setVal(temp.getVal());
    r.storeBigNumber();
}
void bigNum::ExtendedEUCLID(bigNum m)
{
    bigNum Q; bigNum A1("1"); bigNum A2("0"); bigNum A3 = m; bigNum B1("0"); bigNum B2("1"); bigNum B3 = *this;
    bigNum temp1, temp2, temp3;
    bigNum remainder;
    while(B3.getUnits()->back() != 1)
    {
        Q.div(A3, B3, remainder);
        temp1 = A1; temp2 = A2; temp3 = A3;
        A1 = B1; A2 = B2; A3 = B3;
        B1.mul(B1, Q); B1.sub(temp1, B1);
        B2.mul(B2, Q); B2.sub(temp2, B2);
        B3.mul(B3, Q); B3.sub(temp3, B3);
    }
    longNumber = B2.getVal();
    storeBigNumber();
}