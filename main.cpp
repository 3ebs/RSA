#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <chrono>
#include <algorithm>

#define DEBUG           true

using namespace std;
using namespace std::chrono;

class bigNum {
private:
    string longNumber;
    vector<unsigned long> number;
public:
    static unsigned long maxNumber;

    bigNum();

    bigNum(string number);

    void storeBigNumber();

    string getVal();

    void setVal(string s);

    vector<unsigned long> *getUnits();

    void add(bigNum x, bigNum y);

    bool sub(bigNum x, bigNum y);

    void mul(bigNum x, bigNum y);

    void div(bigNum x, bigNum y, bigNum &r);

    void doubleDiv(bigNum x, bigNum y, bigNum &r);

    //void div2(bigNum x, bigNum y, bigNum &r);

    void ExtendedEUCLID(bigNum e, bigNum m);

    bool isOddOrEven();

    bool isPrime(int n);

    char compare(bigNum x, bigNum y);

    void powMod(bigNum x, bigNum y, bigNum z, bigNum phiZ);
};

int main(int argc, char **argv) {
    string p = "P=12369571528747655798110188786567180759626910465726920556567298659370399748072366507234899432827475865189642714067836207300153035059472237275816384410077871";
    string q = "Q=2065420353441994803054315079370635087865508423962173447811880044936318158815802774220405304957787464676771309034463560633713497474362222775683960029689473";
    string e = "E=65537";
    string option;
    //cin >> p >> q >> e;
    p = p.substr(2);
    q = q.substr(2);
    e = e.substr(2);
    bigNum P(p);
    bigNum Q(q);
    bigNum E(e);
    bigNum D;
    bigNum M;
    bigNum RES;
    bigNum N, phiN;
    bigNum remainder;
#if DEBUG
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
#endif
    while(true) {
        cin >> option;
#if DEBUG
        t1 = high_resolution_clock::now();
#endif
        if(option == "IsPPrime") {
            bool isPPrime = P.isPrime(1);
            if (isPPrime) cout << "Yes" << endl;
            else cout << "No" << endl;
        }
        else if(option == "IsQPrime") {
            bool isQPrime = Q.isPrime(1);
            if (isQPrime) cout << "Yes" << endl;
            else cout << "No" << endl;
        }
        else if(option == "PrintN") {
            N.mul(P, Q);
            cout << N.getVal() << endl;
        }
        else if(option == "PrintPhi") {
            D.sub(P, bigNum("1"));
            M.sub(Q, bigNum("1"));
            phiN.mul(D, M);
            cout << phiN.getVal() << endl;
        }
        else if(option == "PrintD") {
            D.sub(P, bigNum("1"));
            M.sub(Q, bigNum("1"));
            phiN.mul(D, M);
            D.ExtendedEUCLID(E, phiN);
            cout << D.getVal() << endl;
        }
        else if(option.substr(7, 6) == "Public") {
            D.sub(P, bigNum("1"));
            RES.sub(Q, bigNum("1"));
            phiN.mul(D, RES);
            N.mul(P, Q);
            option.pop_back();
            M = bigNum(option.substr(14));
            RES.powMod(M, E, N, phiN);
            cout << RES.getVal() << endl;
        }
        else if(option.substr(7, 7) == "Private") {
            D.sub(P, bigNum("1"));
            RES.sub(Q, bigNum("1"));
            phiN.mul(D, RES);
            N.mul(P, Q);
            option.pop_back();
            M = bigNum(option.substr(15));
            D.ExtendedEUCLID(E, phiN);
            RES.powMod(M, D, N, phiN);
            cout << RES.getVal() << endl;
        }
        else if(option == "Quit")
            break;
#if DEBUG
        t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << duration << endl;
#endif
    }
    return 0;
}

unsigned long bigNum::maxNumber = 1000000000;

bigNum::bigNum() {
    longNumber = "0";
    storeBigNumber();
    number.clear();
}

bigNum::bigNum(string number) {
    longNumber = number;
    storeBigNumber();
}

void bigNum::storeBigNumber() {
    number.clear();
    while (longNumber.front() == '0') longNumber = longNumber.substr(1);
    if (longNumber.length() == 0) longNumber = "0";
    unsigned int length = (unsigned int) longNumber.length();
    unsigned int remainder = length % 9;
    if (remainder > 0) number.push_back(stoul(longNumber.substr(0, remainder)));
    for (int i = remainder; i < length; i += 9) {
        number.push_back(stoul(longNumber.substr((unsigned long) i, 9)));
    }
}

string bigNum::getVal() {
    return longNumber;
}

void bigNum::setVal(string s) {
    longNumber = s;
}

vector<unsigned long> *bigNum::getUnits() {
    return &number;
}

char bigNum::compare(bigNum x, bigNum y) {
    //'g' if x > y
    //'l' if x < y
    //'e' if x == y
    string xNumber = x.getVal();
    string yNumber = y.getVal();
    if (xNumber == yNumber) return 'e';
    if (xNumber.length() > yNumber.length()) return 'g';
    else if (xNumber.length() < yNumber.length()) return 'l';
    else if (xNumber.length() == yNumber.length())
        for (int i = 0; i < xNumber.length(); i++) {
            if (xNumber[i] > yNumber[i])
                return 'g';
            else if (xNumber[i] < yNumber[i])
                return 'l';
        }
    return 'n';
}

void bigNum::add(bigNum x, bigNum y) {
    string tempStr;
    string testDigits;
    unsigned long long tempVal;
    vector<unsigned long> *numberX = x.getUnits();
    vector<unsigned long> *numberY = y.getUnits();
    while (numberX->size() > numberY->size()) numberY->insert(numberY->begin(), 0);
    while (numberY->size() > numberX->size()) numberX->insert(numberX->begin(), 0);
    int index = (int) numberX->size() - 1;
    while (index >= 0) {
        tempVal = (*numberX)[index] + (*numberY)[index];
        if (tempVal > maxNumber - 1) {
            tempVal %= maxNumber;
            if (index - 1 >= 0) (*numberX)[index - 1] += 1; //may be error if it's already 2^32 - 1
        }
        testDigits = to_string(tempVal);
        while (testDigits.length() < 9) testDigits.insert(0, "0");
        tempStr.insert(0, testDigits);
        index--;
    }
    longNumber = tempStr;
    storeBigNumber();
}

bool bigNum::sub(bigNum x, bigNum y) {
    string tempStr;
    string testDigits;
    long long tempVal;
    vector<unsigned long> *numberX = x.getUnits();
    vector<unsigned long> *numberY = y.getUnits();
    while (numberX->size() > numberY->size()) numberY->insert(numberY->begin(), 0);
    while (numberY->size() > numberX->size()) numberX->insert(numberX->begin(), 0);
    int index = (int) numberX->size() - 1;
    while (index >= 0) {
        tempVal = (long long) ((*numberX)[index] - (*numberY)[index]);
        if (tempVal < 0) {
            while (tempVal < 0) tempVal += maxNumber;
            if (index - 1 > 0) (*numberX)[index - 1] -= 1;
            else if (index - 1 == 0) {
                if ((*numberX)[0] > 0) (*numberX)[0] -= 1;
                else return false;
            } else return false;
        }
        testDigits = to_string(tempVal);
        while (testDigits.length() < 9) testDigits.insert(0, "0");
        tempStr.insert(0, testDigits);
        index--;
    }
    longNumber = tempStr;
    storeBigNumber();
    return true;
}

void bigNum::mul(bigNum x, bigNum y) {
    string tempStr;
    string tempStrMain;
    string tempStrSec;
    unsigned long long offset = 0;
    unsigned int tempVal;
    vector<unsigned long> *numberX = x.getUnits();
    vector<unsigned long> *numberY = y.getUnits();
    int total_length = (int) (numberX->size() + numberY->size()) - 1;
    vector<unsigned long long> result((unsigned long) total_length, 0);
    for (int i = (int) numberX->size() - 1; i >= 0; i--) {
        for (int j = (int) numberY->size() - 1; j >= 0; j--) {
            result[(i + j)] += (*numberX)[i] * (*numberY)[j];
        }
    }
    for (int i = (int) result.size() - 1; i >= 0; i--) {
        tempStr = to_string(result[i]);
        if (tempStr.length() > 9) {
            tempStrMain = tempStr.substr(tempStr.length() - 9, 9);
            tempStrSec = tempStr.substr(0, tempStr.length() - 9);
        } else {
            tempStrMain = tempStr;
            tempStrSec = "0";
        }
        result[i] = stoul(tempStrMain) + offset;
        if (result[i] > maxNumber - 1) {
            tempVal = (unsigned int) (result[i] / maxNumber);
            result[i] %= maxNumber;
            offset = stoul(tempStrSec) + tempVal;
        } else {
            offset = stoul(tempStrSec);
        }
    }
    if (offset > 0) {
        result.insert(result.begin(), 0);
        result[0] = offset;
    }
    longNumber.clear();
    for (int i = 0; i < result.size(); i++) {
        tempStr = to_string(result[i]);
        while (tempStr.length() < 9) tempStr.insert(0, "0");
        longNumber.append(tempStr);
    }
    storeBigNumber();
}

void bigNum::div(bigNum x, bigNum y, bigNum &r) {
    unsigned long long result = 0;
    string RES = "";
    string xNumber = x.getVal();
    string yNumber = y.getVal();
    string newX;
    string newTemp;
    bool lFlag = false;
    bigNum temp;
    if (yNumber == "0") {
        longNumber = "0";
        storeBigNumber();
        r.setVal("1");
        r.storeBigNumber();
        return;
    }
    if (xNumber == "0") {
        longNumber = "0";
        storeBigNumber();
        r.setVal("0");
        r.storeBigNumber();
        return;
    }
    if (compare(x, y) == 'g')
        if(xNumber[0] >= yNumber[0])
            lFlag = true;
        else
            lFlag = false;
    else if (compare(x, y) == 'l') {
        longNumber = "0";
        storeBigNumber();
        r = x;
        return;
    }
    else {
        longNumber = "1";
        storeBigNumber();
        r.setVal("0");
        r.storeBigNumber();
        return;
    }
    if (lFlag)
        newX = xNumber.substr(0, yNumber.length());
    else
        newX = xNumber.substr(0, yNumber.length() + 1);
    if(yNumber.length() <= 18) {
        if(xNumber.length() >= 18) {
            newX = xNumber.substr(0, 18);
            if (xNumber[18] == '0' && 18 != xNumber.length()-1)
                newX = xNumber.substr(0, 17);
        }
        else
            newX = xNumber;
    }
    temp.setVal(newX);
    temp.storeBigNumber();
    for (int i = (int) newX.length(); i < xNumber.length(); i++) {
        if (yNumber.length() <= 18 && temp.getVal().length() <= 18) {

            result = stoull(temp.getVal()) / stoull(yNumber);
            temp.setVal(to_string(stoull(temp.getVal()) % stoull(yNumber)));
            temp.storeBigNumber();
        }
        else
            while (temp.sub(temp, y))
                result++;
        RES.append(to_string(result));
        //cout << RES.length() << endl;
        result = 0;
//        while (temp.getVal().length() < yNumber.length() && xNumber[i] != NULL) {
            temp.setVal(temp.getVal() + xNumber[i]);
//            i++;
//        }
//        while (xNumber[i] == '0' && i != xNumber.length()-1) {
//            newX = temp.getVal();
//            newX.pop_back();
//            temp.setVal(newX);
//            i--;
//        }
//        i--;
        temp.storeBigNumber();
    }
    if (yNumber.length() <= 18 && temp.getVal().length() <= 18) {

        result = stoull(temp.getVal()) / stoull(yNumber);
        temp.setVal(to_string(stoull(temp.getVal()) % stoull(yNumber)));
        temp.storeBigNumber();
    }
    else
        while (temp.sub(temp, y))
            result++;
    RES.append(to_string(result));
    longNumber = RES;
    storeBigNumber();
    r.setVal(temp.getVal());
    r.storeBigNumber();
}

/*void bigNum::div2(bigNum x, bigNum y, bigNum &r) {
    unsigned long long result = 0;
    string RES = "";
    string xNumber = x.getVal();
    string yNumber = y.getVal();
    string newX;
    string newTemp;
    bool lFlag = false;
    bigNum temp;
    if (yNumber == "0") {
        longNumber = "0";
        storeBigNumber();
        r.setVal("1");
        r.storeBigNumber();
        return;
    }
    if (xNumber == "0") {
        longNumber = "0";
        storeBigNumber();
        r.setVal("0");
        r.storeBigNumber();
        return;
    }
    if (compare(x, y) == 'g')
        if(xNumber[0] >= yNumber[0])
            lFlag = true;
        else
            lFlag = false;
    else if (compare(x, y) == 'l') {
        longNumber = "0";
        storeBigNumber();
        r = x;
        return;
    }
    else {
        longNumber = "1";
        storeBigNumber();
        r.setVal("0");
        r.storeBigNumber();
        return;
    }
    if (lFlag)
        newX = xNumber.substr(0, yNumber.length());
    else
        newX = xNumber.substr(0, yNumber.length() + 1);
    temp.setVal(newX);
    temp.storeBigNumber();
    for (int i = (int) newX.length(); i < xNumber.length(); i++) {
        while (temp.sub(temp, y))
            result++;
        RES.append(to_string(result));
        result = 0;
        temp.setVal(temp.getVal() + xNumber[i]);
        temp.storeBigNumber();
    }
    while (temp.sub(temp, y))
        result++;
    RES.append(to_string(result));
    longNumber = RES;
    storeBigNumber();
    r.setVal(temp.getVal());
    r.storeBigNumber();
}*/

void bigNum::doubleDiv(bigNum x, bigNum y, bigNum &r) {
    bigNum accumResult("0");
    bigNum chosenNum;
    vector<bigNum> temp(4);
    bigNum tempNum;
    temp[3] = bigNum("1"+y.getVal());
    temp[2].mul(y, bigNum("2"));
    temp[2].setVal("2"+temp[2].getVal());
    temp[2].storeBigNumber();
    temp[1].mul(y, bigNum("4"));
    temp[1].setVal("4"+temp[1].getVal());
    temp[1].storeBigNumber();
    temp[0].mul(y, bigNum("8"));
    temp[0].setVal("8"+temp[0].getVal());
    temp[0].storeBigNumber();
    sort(begin(temp), end(temp), [](bigNum &a, bigNum &b) {return a.getVal()[1] > b.getVal()[1];});
    string xNumber = x.getVal();
    string yNumber = y.getVal();
    string tempNumber;
    string chosenNumber;
    bool remainderFlag = false;
    if (yNumber == "0") {
        longNumber = "0";
        storeBigNumber();
        r.setVal("1");
        r.storeBigNumber();
        return;
    }
    if (xNumber == "0") {
        longNumber = "0";
        storeBigNumber();
        r.setVal("0");
        r.storeBigNumber();
        return;
    }
    if (compare(x, y) == 'l') {
        longNumber = "0";
        storeBigNumber();
        r = x;
        return;
    }
    else if (compare(x, y) == 'e') {
        longNumber = "1";
        storeBigNumber();
        r.setVal("0");
        r.storeBigNumber();
        return;
    }
    bool myFlag = false;
    char counter = 0;
    while (!remainderFlag) {
        remainderFlag = true;
        for (int i = 0; i < 4; i++) {
            tempNum.setVal(x.getVal().substr(0, temp[i].getVal().length()-1));
            tempNum.storeBigNumber();
            counter++;
            if (compare(bigNum(temp[i].getVal().substr(1)), tempNum) == 'l' || compare(bigNum(temp[i].getVal().substr(1)), tempNum) == 'e' || myFlag) {
                counter = 0;
                tempNumber = temp[i].getVal().substr(1);
                chosenNumber = to_string(temp[i].getVal()[0]-'0');
                if(myFlag) {
                    while (tempNumber.length() < x.getVal().length()-1) {
                        tempNumber += '0';
                        chosenNumber += '0';
                    }
                    myFlag = false;
                }
                else {
                    while (tempNumber.length() < x.getVal().length()) {
                        tempNumber += '0';
                        chosenNumber += '0';
                    }
                }
                chosenNum.setVal(chosenNumber);
                chosenNum.storeBigNumber();
                accumResult.add(accumResult, chosenNum);
                x.sub(x, bigNum(tempNumber));
                remainderFlag = false;
                break;
            }
        }
        if(counter == 4) {
            counter = 0;
            for (int i = 0; i < 4; i++) {
                if (x.getVal().length() > temp[i].getVal().substr(1).length()) {
                    remainderFlag = false;
                    myFlag = true;
                    break;
                }
            }
        }
    }
    r = x;
    *this = accumResult;
}

void bigNum::ExtendedEUCLID(bigNum e, bigNum m) {
    bigNum Q;
    bigNum A2("0");
    bigNum A3 = m;
    bigNum B2("1");
    bigNum B3 = e;
    bigNum temp2, temp3;
    bigNum remainder;
    while (B3.getUnits()->back() != 1) {
        Q.div(A3, B3, remainder);
        temp2 = A2;
        temp3 = A3;
        A3 = B3;
        A2 = B2;
        temp3.div(temp3, B3, B3);
        B2.mul(B2, Q);
        while (!(B2.sub(temp2, B2))) {
            temp2.add(temp2, m);
        }
        temp2.div(B2, m, B2);
    }
    longNumber = B2.getVal();
    storeBigNumber();
}

bool bigNum::isOddOrEven() {
    char firstDigit = longNumber[longNumber.length() - 1];
    int first_digit = firstDigit - '0';
    return first_digit % 2 == 0;
}

void bigNum::powMod(bigNum x, bigNum y, bigNum z, bigNum phiZ) {
    bigNum res("1");
    bigNum temp;
    if (phiZ.getVal() != "0") temp.div(y, phiZ, y);
    while (y.getVal() != "0") {
        if (!y.isOddOrEven()) {
            res.mul(res, x);
            temp.div(res, z, res);
        }
        x.mul(x, x);
        temp.div(x, z, x);
        y.div(y, bigNum("2"), temp);
    }
    longNumber = res.getVal();
    storeBigNumber();
}

bool bigNum::isPrime(int n) {
    if(longNumber == "1" || longNumber == "4") return false;
    else if(longNumber == "2" || longNumber == "3") return true;
    if(isOddOrEven()) return false;
    int k = 0;
    char primeCount = 0;
    bigNum temp;
    bigNum tempA;
    bigNum q;
    vector<bigNum> a;
    bigNum this_1;
    this_1.sub(*this, bigNum("1"));
    q = this_1;
    bigNum remainder;
    while(q.isOddOrEven()) {
        q.div(q, bigNum("2"), remainder);
        k++;
    }
    string randomNum;
    int digitCount = (int)longNumber.length() / 2;
    for (int l = 0; l < n; ++l) {
        randomNum = "";
        for(int i = 0; i < digitCount; i++) {
            randomNum += to_string(rand() % 10 + 2);
        }
        while(randomNum.length() > digitCount) randomNum.pop_back();
        a.push_back(bigNum(randomNum));
    }

    for (int j = 0; j < n; ++j) {
        temp.powMod(a[j], q, *this, bigNum("0"));
        if(compare(temp, bigNum("1")) == 'e' || compare(temp, this_1) == 'e') {
            primeCount++;
            continue;
        }
        for (int i = 0; i < k-1; ++i) {
            temp.mul(temp, temp);
            remainder.div(temp, *this, temp);
            if(compare(temp, this_1) == 'e') {
                primeCount++;
                break;
            }
        }
    }
    if(primeCount == n) return true;
    else return false;
}