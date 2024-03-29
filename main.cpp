#include <iostream>
#include <vector>
#include <chrono>

#define DEBUG           true
#define USE_DEFAULT     false

using namespace std;
using namespace std::chrono;

unsigned long maxNumber = 1000000000;

class bigNum {
private:
    string longNumber;
    vector<unsigned long> number;
public:
    bigNum();
    bigNum(string number);
    void storeBigNumber();
    inline string getVal() const;
    void setVal(string s);
    inline vector<unsigned long> getUnits() const;
    void add(const bigNum &x, const bigNum &y);
    bool sub(const bigNum &x, const bigNum &y);
    void mul(const bigNum &x, const bigNum &y);
    void div(const bigNum &x, const bigNum &y, bigNum &r);
    void powMod(const bigNum &a, const bigNum &b, const bigNum &z, const bigNum &phiZ);
    void ExtendedEUCLID(const bigNum &e, const bigNum &m);
    bool isOddOrEven();
    bool isPrime(int n, int _random=1);
    bool operator==(const bigNum &x)const;
    bool operator<=(const bigNum &x)const;
    bool operator>=(const bigNum &x)const;
    bool operator<(const bigNum &x)const;
    bool operator>(const bigNum &x)const;
};

bigNum zero("0");
bigNum one("1");
bigNum two("2");

int main(int argc, char **argv) {
    string p;
    string q;
    string e;
    string option;
#if USE_DEFAULT
    p = "P=12369571528747655798110188786567180759626910465726920556567298659370399748072366507234899432827475865189642714067836207300153035059472237275816384410077871";
    q = "Q=2065420353441994803054315079370635087865508423962173447811880044936318158815802774220405304957787464676771309034463560633713497474362222775683960029689473";
    e = "E=65537";
#endif
#if !USE_DEFAULT
    cin >> p >> q >> e;
#endif
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
    while (true) {
        cin >> option;
#if DEBUG
        t1 = high_resolution_clock::now();
#endif
        if(option == "IsPPrime") {
            bool isPPrime = P.isPrime(2, 0);
            if (isPPrime) cout << "Yes" << endl;
            else cout << "No" << endl;
        }
        else if(option == "IsQPrime") {
            bool isQPrime = Q.isPrime(2, 0);
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
        else if(option == "Quit")
            break;
        else if(option.substr(7, 6) == "Public") {
            D.sub(P, bigNum("1"));
            RES.sub(Q, bigNum("1"));
            phiN.mul(D, RES);
            N.mul(P, Q);
            option.pop_back();
            M = bigNum(option.substr(15));
            RES.powMod(M, E, N, phiN);
            cout << RES.getVal() << endl;
        }
        else if(option.substr(7, 7) == "Private") {
            D.sub(P, bigNum("1"));
            RES.sub(Q, bigNum("1"));
            phiN.mul(D, RES);
            N.mul(P, Q);
            option.pop_back();
            M = bigNum(option.substr(16));
            D.ExtendedEUCLID(E, phiN);
            RES.powMod(M, D, N, phiN);
            cout << RES.getVal() << endl;
        }
#if DEBUG
        t2 = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(t2 - t1).count();
        cout << "execution time = " << duration << " seconds" << endl;
#endif
    }
    return 0;
}


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
    if (remainder > 0)
        number.push_back(stoul(longNumber.substr(0, remainder)));
    for (int i = remainder; i < length; i += 9)
        number.push_back(stoul(longNumber.substr((unsigned long) i, 9)));
}

string bigNum::getVal() const {
    return longNumber;
}

void bigNum::setVal(string s) {
    longNumber = s;
}

vector<unsigned long> bigNum::getUnits() const{
    return number;
}

bool bigNum::operator==(const bigNum &x) const {
    return x.getVal() == this->getVal();
}

bool bigNum::operator<=(const bigNum &x) const {
    string xNumber = x.getVal();
    string yNumber = this->getVal();
    if(*this == x) return true;
    if (xNumber.length() > yNumber.length()) return true;
    if (xNumber.length() == yNumber.length())
        for (int i = 0; i < xNumber.length(); ++i)
            if (xNumber[i] > yNumber[i])
                return true;
    return false;
}

bool bigNum::operator>=(const bigNum &x) const {
    string xNumber = x.getVal();
    string yNumber = this->getVal();
    if(*this == x) return true;
    if (xNumber.length() < yNumber.length()) return true;
    if (xNumber.length() == yNumber.length())
        for (int i = 0; i < xNumber.length(); ++i)
            if (xNumber[i] < yNumber[i])
                return true;
    return false;
}

bool bigNum::operator<(const bigNum &x) const {
    string xNumber = x.getVal();
    string yNumber = this->getVal();
    if (xNumber.length() > yNumber.length()) return true;
    if (xNumber.length() == yNumber.length())
        for (int i = 0; i < xNumber.length(); ++i)
            if (xNumber[i] > yNumber[i])
                return true;
    return false;
}

bool bigNum::operator>(const bigNum &x) const {
    string xNumber = x.getVal();
    string yNumber = this->getVal();
    if (xNumber.length() < yNumber.length()) return true;
    if (xNumber.length() == yNumber.length())
        for (int i = 0; i < xNumber.length(); ++i)
            if (xNumber[i] < yNumber[i])
                return true;
    return false;
}

void bigNum::add(const bigNum &x, const bigNum &y) {
    string tempStr;
    string testDigits;
    unsigned long long tempVal;
    vector<unsigned long> numberX = x.getUnits();
    vector<unsigned long> numberY = y.getUnits();
    while (numberX.size() > numberY.size()) numberY.insert(numberY.begin(), 0);
    while (numberY.size() > numberX.size()) numberX.insert(numberX.begin(), 0);
    int index = (int) numberX.size() - 1;
    while (index >= 0) {
        tempVal = (numberX)[index] + (numberY)[index];
        if (tempVal > maxNumber - 1) {
            tempVal %= maxNumber;
            if (index - 1 >= 0) (numberX)[index - 1] += 1; //may be error if it's already 2^32 - 1
        }
        testDigits = to_string(tempVal);
        while (testDigits.length() < 9) testDigits.insert(0, "0");
        tempStr.insert(0, testDigits);
        index--;
    }
    longNumber = tempStr;
    storeBigNumber();
}

bool bigNum::sub(const bigNum &x, const bigNum &y) {
    string tempStr;
    string testDigits;
    long long tempVal;
    vector<unsigned long> numberX = x.getUnits();
    vector<unsigned long> numberY = y.getUnits();
    while (numberX.size() > numberY.size()) numberY.insert(numberY.begin(), 0);
    while (numberY.size() > numberX.size()) numberX.insert(numberX.begin(), 0);
    int index = (int) numberX.size() - 1;
    while (index >= 0) {
        tempVal = (long long) ((numberX)[index] - (numberY)[index]);
        if (tempVal < 0) {
            while (tempVal < 0) tempVal += maxNumber;
            if (index - 1 > 0) (numberX)[index - 1] -= 1;
            else if (index - 1 == 0) {
                if ((numberX)[0] > 0) (numberX)[0] -= 1;
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

void bigNum::mul(const bigNum &x, const bigNum &y) {
    string tempStr;
    string tempStrMain;
    string tempStrSec;
    unsigned long long offset = 0;
    unsigned int tempVal;
    vector<unsigned long> numberX = x.getUnits();
    vector<unsigned long> numberY = y.getUnits();
    int total_length = (int) (numberX.size() + numberY.size()) - 1;
    vector<unsigned long long> result((unsigned long) total_length, 0);
    for (int i = (int) numberX.size() - 1; i >= 0; i--) {
        for (int j = (int) numberY.size() - 1; j >= 0; j--) {
            result[(i + j)] += (numberX)[i] * (numberY)[j];
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

void bigNum::div(const bigNum &x, const bigNum &y, bigNum &r) {
    unsigned long long result = 0;
    string RES = "";
    string xNumber = x.getVal();
    string yNumber = y.getVal();
    string newX;
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
    if (x < y) {
        longNumber = "0";
        storeBigNumber();
        r = x;
        return;
    }
    if(x == y) {
        longNumber = "1";
        storeBigNumber();
        r.setVal("0");
        r.storeBigNumber();
        return;
    }
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
        result = 0;
        temp.setVal(temp.getVal() + xNumber[i]);
        temp.storeBigNumber();
    }
    if (yNumber.length() <= 18 && temp.getVal().length() <= 18) {

        result = stoull(temp.getVal()) / stoull(yNumber);
        temp.setVal(to_string(stoull(temp.getVal()) % stoull(yNumber)));
        temp.storeBigNumber();
        RES.append(to_string(result));
    }
    else {
        while (temp.sub(temp, y))
            result++;
        RES.append(to_string(result));
    }
    longNumber = RES;
    storeBigNumber();
    r.setVal(temp.getVal());
    r.storeBigNumber();
}

void bigNum::ExtendedEUCLID(const bigNum &e, const bigNum &m) {
    bigNum Q;
    bigNum A2("0");
    bigNum A3 = m;
    bigNum B2("1");
    bigNum B3 = e;
    bigNum temp2, temp3;
    bigNum remainder;
    while (B3.getUnits().back() != 1) {
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

void bigNum::powMod(const bigNum &a, const bigNum &b, const bigNum &z, const bigNum &phiZ) {
    bigNum res("1");
    bigNum x = a;
    bigNum y = b;
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

bool bigNum::isPrime(int n, int _random) {
    if(longNumber == "1" || longNumber == "4")
        return false;
    else if(longNumber == "2" || longNumber == "3")
        return true;
    if(isOddOrEven()) return false;
    int k = 0;
    char primeCount = 0;
    bigNum temp;
    bigNum tempA;
    bigNum q;
    vector<bigNum> a(n);
    bigNum this_1;
    this_1.sub(*this, one);
    q = this_1;
    bigNum remainder;
    while(q.isOddOrEven()) {
        q.div(q, two, remainder);
        k++;
    }
    if(_random) {
        string randomNum;
        int digitCount = (int) longNumber.length() / 2;
        for (int l = 0; l < n; ++l) {
            randomNum = "";
            for (int i = 0; i < digitCount; i++) {
                randomNum += to_string(rand() % 10 + 2);
            }
            while (randomNum.length() > digitCount) randomNum.pop_back();
            a[l] = (bigNum(randomNum));
        }
    }
    else {
        for(int i= 2; i < 2+n; ++i) {
            a[i-2] = bigNum(to_string(i));
        }
    }
    for (int j = 0; j < n; ++j) {
        temp.powMod(a[j], q, *this, zero);
        if(temp == one || temp == this_1) {
            primeCount++;
            continue;
        }
        for (int i = 0; i < k-1; ++i) {
            temp.mul(temp, temp);
            remainder.div(temp, *this, temp);
            if(temp == this_1) {
                primeCount++;
                break;
            }
        }
    }
    if(primeCount)
        return true;
    else
        return false;
}
