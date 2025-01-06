#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
using namespace std;

class Account
{
private :
    string f_name, l_name;
    unsigned int acc_no;
    int pin,age;
    string gender;
    unsigned long int mob_no;
    double acc_balance = 0;
    vector<vector<string>> content;
    int row_no;
public :
    
    void get_content()
    {
        fstream fin;
        vector<string> row;
	    string line, word;
        bool found=0;
        content.clear();
        
        fin.open("Bank_accounts.csv", ios::in);

        if(fin.is_open())
        {
            while(getline(fin, line))
            {
                row.clear();

                stringstream str(line);

                while(getline(str, word, ','))
                {
                    row.push_back(word);
                }
                content.push_back(row);
            }
        }

        fin.close();
    }

    bool isNumber(const string& s)
    {
        for (char const &ch : s)
        {
            if (std::isdigit(ch) == 0) 
                return false;
        }
        return true;
    }

    int sign_up()
    {
        string acc_nocheck, agecheck, mob_nocheck, pinc, checkpin;
        bool check;
        cout << "Enter your first name: ";
        cin >> f_name;
        cout << "Enter your last name: ";
        cin >> l_name;
        cout << "Enter account number(Natural number): ";
        cin >> acc_nocheck;

        check = isNumber(acc_nocheck);
        if(check == 0 || stoi(acc_nocheck)<0)
        {
            cout << "Account number must be natural number please sign-up again." << endl;
            return 0;
        }

        if(acc_nocheck == "1992458621")
        {
            cout << "Not possible to take this acc_no" << endl;
            return 0;
        }

        for(int i=0; i<content.size() ; i++)
        {
            if(acc_nocheck == content[i][0])
            {
                cout << "Account number already taken. Please choose another number." << endl;
                return 0;
            }
        }

        cout << "Enter Age(Natural number): ";
        cin >> agecheck;
        check = isNumber(agecheck);

        if(check == 0 || stoi(agecheck)<0)
        {
            cout << "Age must be natural number please sign-up again." << endl;
            return 0;
        }

        if (stoi(agecheck)<18)
        {
            cout << "Age must be above 18years" << endl;
            return 0;
        }

        cout << "Enter Gender(M or F or T[third gender]): ";
        cin >> gender;
        cout << "Enter mobile number(Natural number)";
        cin >> mob_nocheck;
        check = isNumber(mob_nocheck);

        if(check == 0 || stoi(mob_nocheck)<0)
        {
            cout << "Mobile number must be natural number please sign-up again." << endl;
            return 0;
        }

        cout << "Enter pin(Natural number)";
        cin >> pinc;
        check = isNumber(pinc);

        if(check == 0 || stoi(pinc)<0)
        {
            cout << "Pin must be natural number please sign-up again." << endl;
            return 0;
        }

        cout << "Confirm pin";
        cin >> checkpin;

        if (pinc != checkpin)
        {
            cout << "Both the pins should be same, Please try again." << endl;
            return 0;
        }

        acc_no = stoi(acc_nocheck);
        age = stoi(agecheck);
        mob_no = stoi(mob_nocheck);
        pin = stoi(pinc);

        fstream fout;
        fout.open("Bank_accounts.csv", ios::out | ios::app);

        fout << acc_no << ","
             << f_name << ","
             << l_name << ","
             << age << ","
             << gender << ","
             << mob_no << ","
             << acc_balance << ","
             << pin << "\n";

        fout.close();
        return 0;
    }

    void login()
    {
        fstream fin;
        string checkacc_no;
        string ch_pin;
        cout << "Enter account number to check its details: ";
        cin >> checkacc_no;
        cout << "Enter pin: ";
        cin >> ch_pin;
        
        bool found=0;

        for(int i=0; i<content.size() ; i++)
        {
            if(content[i][0] == checkacc_no)
            {
                found = 1;
                if(content[i][7] == ch_pin)
                {                        
                    acc_no = stoi(content[i][0]);
                    f_name = content[i][1];
                    l_name = content[i][2];
                    age = stoi(content[i][3]);
                    gender = content[i][4];
                    mob_no = stoi(content[i][5]);                        
                    acc_balance = stoi(content[i][6]);
                    row_no = i;
                }
                else
                {
                    cout << "Incorrect Password. Try Again" << endl;
                    break;
                }
            }
        }

        if(found == 0)
        {
            cout << "Account Number Not Present." << endl;
        }

        fin.close();
    }

    void display_details()
    {  
        cout << "Acc number : " << acc_no << endl;
        cout << "Name       : " << f_name << " " << l_name << endl;
        cout << "Age        : " << age << endl;
        cout << "Gender     : " << gender << endl;
        cout << "Mobile no  : " << mob_no << endl;
        cout << "Balance    : " << acc_balance << endl;
    }

    void dis_balance()
    {
        acc_balance = stoi(content[row_no][6]);
        cout << "Balance : " << acc_balance << endl;
    }

    void update_records()
    {
        fstream fout;
        remove("Bank_accounts.csv");
        fout.open("Bank_accounts.csv", ios::out);
        
        for(int i =0; i<content.size() ; i++)
        {
            for(int j=0; j<content[i].size() ; j++)
            {
                fout << content[i][j] << ",";
            }
            if (i = content.size()-1)
                break;
            fout << "\n";
        }
        fout.close();
    }

    int deposit()
    {
        string deposit;
        acc_balance = stoi(content[row_no][6]);
        cout << "Enter money to deposit: ";
        cin >> deposit;

        if(!isNumber(deposit) || stoi(deposit)<0)
        {
            cout << "Deposit must be natural number" << endl;
            return 0;
        }

        acc_balance += stoi(deposit);
        content[row_no][6] = to_string(acc_balance);
        update_records();
        dis_balance();

        fstream fout;
        fout.open("Transaction.csv", ios::out | ios::app);
        fout << "d" << ","
             << acc_no << ","
             << deposit << ","
             << endl ;
        fout.close();
        return 0;
    }

    void withdraw()
    {
        float withdraw;
        acc_balance = stoi(content[row_no][6]);
        cout << "Balance : " << acc_balance << endl;
        cout << "Enter money to withdraw: ";
        cin >> withdraw;
        acc_balance -= withdraw;

        if(acc_balance >= 0)
        {
            cout << "Balance after withdraw: " << acc_balance << endl;
            content[row_no][6] = to_string(acc_balance);
            update_records();
            fstream fout;
            fout.open("Transaction.csv", ios::out | ios::app);
            fout << "w" << ","
                 << acc_no << ","
                 << withdraw << ","
                 << endl  ;
                 
            fout.close();
        }
        else
        {
            cout << "Insufficient balance, Balance should remain positive" << endl;
            acc_balance += withdraw;
        }
    }

    int transfer()
    {
        string acc_numsend, amt_transferstr;
        int rankrow, check=1;
        cout << "Enter acc_no of who you want to transfer: ";
        cin >> acc_numsend;

        if(!isNumber(acc_numsend))
        {
            cout << "Account number is natural number" << endl;
            return 0;
        }

        cout << "Enter amount to transfer: ";
        cin >> amt_transferstr;

        if(!isNumber(amt_transferstr))
        {
            cout << "Amount is a natural number" << endl;
            return 0;
        }
        int amt_transfer = stoi(amt_transferstr);

        for (int i=0; i<content.size(); i++)
        {
            if(content[i][0] == acc_numsend)
            {
                rankrow = i;
                check = 0;
                break;
            }
        }

        if(check == 1)
        {
            cout << "Account does not exist." << endl;
            return 0;
        }

        if(stoi(content[row_no][6]) >= amt_transfer)
        {
            acc_balance -= amt_transfer;
            content[row_no][6] = to_string(acc_balance);
        }
        else
        {
            cout << "Amount wanted to transfer exceeds account balance. Not possible";
            return 0;
        }

        content[rankrow][6] = to_string(stoi(content[rankrow][6]) + amt_transfer);
        cout << content[rankrow][6];
        fstream fout;
        fout.open("Transaction.csv", ios::out | ios::app);
        fout << "t" << ","
             << acc_no << ","
             << amt_transfer << "," 
             << acc_numsend << ","
             << endl ;
        fout.close();

        update_records();
        return 0;
    }

    bool check_login()
    {
        if(acc_no == 1992458621)
            return 1;
        else
            return 0;
    }

    void tran_history()
    {
        // Write code here for printing transaction history of that dude
        vector<vector<string>> t_content;
        vector<string> row;
	    string line, word;
        bool found=0;
        t_content.clear();

        fstream fin;
        fin.open("Transaction.csv", ios::in);

        if(fin.is_open())
        {
            while(getline(fin, line))
            {
                row.clear();

                stringstream str(line);

                while(getline(str, word, ','))
                {
                    row.push_back(word);
                }
                t_content.push_back(row);
            }
        }

        for (int i=0; i<(t_content.size()); i++)
        {
            if(stoi(t_content[i][1]) == acc_no)
            {
                if((t_content[i][0]) == "d")
                {
                    cout << t_content[i][2] << "Rs - Deposited "<< endl;
                }
                if((t_content[i][0]) == "w")
                {
                    cout << t_content[i][2] << "Rs - Withdraw "<< endl;
                }
                if((t_content[i][0]) == "t")
                {
                    cout << t_content[i][2] << "Rs - Transfered to " << t_content[i][3]<< endl;
                }
            }
            if(t_content[i].size() == 4)
            {
                if(stoi(t_content[i][3]) == acc_no)
                {
                    cout << t_content[i][2] << "Rs - " << t_content[i][1] << " transferred you"<< endl;
                }
            }
        }

        fin.close();
    }

    void logout()
    {
        acc_no = 0;
        f_name = "0";
        l_name = "0";
        age = 0;
        gender = "0";
        mob_no = 0;
        acc_balance = 0;
        row_no = 1000;
    }

};

int main()
{
    Account client;
    int choice;
    string choicestr;
    bool loop = 1;
    bool notlogin = 1;

    while (loop)
    {
        client.get_content();
        if(notlogin)
        {
            cout << "Choose your Choice" << endl;
            cout << "Enter 1 to Sign-up" << endl;
            cout << "Enter 2 to Login" << endl;
            cout << "Enter 3 to Exit" << endl;
            cin >> choicestr;
            if(client.isNumber(choicestr))
                choice = stoi(choicestr);
            else
            {
                cout << "Enter any of the choice numbers." << endl;
                continue;
            }
             switch (choice)
            {
                case 1: 
                    client.sign_up();
                    break;
                case 2:
                    client.login();
                    notlogin = client.check_login();
                    break;
                case 3:
                    loop = 0;
                    break;
                default:
                    cout << "Enter valid choice" << endl;
                    break;
            }
        }

        else
        {
            cout << "Choose your Choice" << endl;
            cout << "Enter 1 to Display Details" << endl;
            cout << "Enter 2 to Display balance" << endl;
            cout << "Enter 3 to Deposit" << endl;
            cout << "Enter 4 to Withdraw" << endl;
            cout << "Enter 5 to Transfer" << endl;
            cout << "Enter 6 to Logout" << endl;
            cout << "Enter 7 to Transaction History" << endl;
            cout << "Enter 8 to Exit" << endl;
            cin >> choicestr;
            if(client.isNumber(choicestr))
                choice = stoi(choicestr);
            else
            {
                cout << "Enter any of the choice numbers." << endl;
                continue;
            }
            switch (choice)
            {
                case 1:
                    client.display_details();
                    break;
                case 2:
                    client.dis_balance();
                    break;
                case 3:
                    client.deposit();
                    break;
                case 4:
                    client.withdraw();
                    break;
                case 5:
                    client.transfer();
                    break;
                case 6:
                    client.logout();
                    notlogin=1;
                    break;
                case 7:
                    client.tran_history();
                    break;
                case 8:
                    loop = 0;
                    break;
                default:
                    cout << "Invalid response";
                    break;
            }
        }
    }
    return 0;
}
