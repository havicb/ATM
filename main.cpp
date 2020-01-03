#include <iostream>
#include <string>
#include <vector>
#include <memory>


using namespace std;

class Date {
    int d, m, y;
public:
    Date (int d = 27, int m = 6, int y = 2019); // UZET CU DANASNJI DATUM
    int getDay () const {return d;}
    int getMonth () const {return m;}
    int getYear () const {return y;}
    void PrintDate () const {
    cout << d << "/" << m << "/" << y <<endl;
    };
};


class Transaction {
    protected:
    double deposit;
    Date transactionDate;
    string payer;
public:
    Transaction (Date d, string payer, double deposit) : transactionDate(d), payer(payer), deposit(deposit) {};
    virtual void getAbout () const {
        cout << "Transakciju obavio: " << payer <<endl;
        cout << "Datum transakcije: "; transactionDate.PrintDate();
        cout << "Iznos: " << deposit <<endl;
    }
};

class RecievedTransactions : public Transaction {
public:
    RecievedTransactions(Date d, string payer, double deposit) : Transaction(d, payer, deposit) {};
    void getAbout () const override {
        cout << "Uplaceno na vas racun!" <<endl;
        Transaction::getAbout();
    }
};

class SentTransactions : public Transaction {
public:
    SentTransactions(Date d, string payer, double deposit) : Transaction(d, payer, deposit) {};
    void getAbout () const override {
        cout << "Skinuto sa vaseg racuna!" <<endl;
        Transaction::getAbout();
    }
};


class Account {
    vector<SentTransactions> sentTransactions;
    vector<RecievedTransactions> gotTransactions;
    double deposit;
    string name, surname, username, password, paket, valut, currency;
    void incijalizirajPaket (char znak);
public:
    Account (string name, string surname, string username, string password, double deposit, char znak, string currency);
    Account& raiseAmount (double amount, string currency);
    Account& addTransaction (Date d, string name, double dep, char znak);
    Account& addAmount (double amount, string currency);
    Account& changePassword (string newPassword);
    void setDeposit (double nDeposit) {deposit = nDeposit;}
    void setCurrency (string nCurrency) {currency = nCurrency;}
    string getUsername() const {return username;}
    string getName () const {return name;}
    string getPassword() const {return password;}
    string getCurrency () const {return currency;}
    double getDeposit () const { return deposit;};
    void seeTransactions () const;
    void aboutAcc () const;
};

class Bank {
    vector<shared_ptr<Account> > accounts;
public:
    Bank () : accounts(0) {};
    Bank (const Bank &b);
    Bank& operator= (const Bank &b);
    Bank& createAccount(string name, string surname, string username, string password, double deposit, char znak, string currency); // PROCITAH DA JE BOLJE PRIMITI OBJEKAT DA BI SE SMANJI BROJ PARAMETARA PA AKO MZOES DATI MISLJENJE ///
    Bank& loginAccount (string username, string password, int &indexRacuna);
    Bank &changePassword (string username, string oldPass, string newPass);
    Bank& addAmount (string username, string pass, double deposit, string currency);
    Bank& transferAmount (string username, string pass, string otherUsername, double deposit, string currency);
    Bank& raiseMoney (string username, string pass, double deposit, string currency);
    Bank& changeCurrency (string username, string pass, string wantedCurrency);
    int getAccID(string username, string pass) const;
    bool getAcc (string username, int &index);
    void seeDeposit (string username, string password);
    void seeTransactions (string username, string password);
    void seeAccounts (string name);
};

Date::Date(int d, int m, int y) {
    int days[]{31,28,30,31,30,31,30,30,30,31,30,31};
        if (d < 1 || d > days[m-1] || m < 1 || m > 12 || y < 1940)
            throw range_error ("Incorrect date!");
this->d = d; this->m = m; this->y = y;
}

void convertValute (double &amount, string &enteredCurrency, string wantedCurrency) {
    if (enteredCurrency == wantedCurrency)
        return;
    if (enteredCurrency == "KM") {
        if (wantedCurrency == "E")
            amount *= 0.5114;
            enteredCurrency = wantedCurrency;
        if (wantedCurrency == "USD")
            amount *= 0.5814;
    cout << "Uspjesno ste promjenili valutu!" <<endl;
    }
    else if (enteredCurrency == "E") {
        if (wantedCurrency == "USD")
            amount*=1.1369;
        if (wantedCurrency == "KM")
            amount*=1.957; //
    cout << "Uspjesno ste promjenili valutu!" <<endl;
    }
    else if (enteredCurrency == "USD") {
        if (wantedCurrency == "KM")
            amount*=1.72;
        if (wantedCurrency == "E")
            amount*=0.8796;
    cout << "Uspjesno ste promjenili valutu!" <<endl;
    }
    enteredCurrency = wantedCurrency;
}

bool checkCurrency (string currency) {
return (currency == "KM" || currency == "E" || currency == "USD");
}

bool checkPassword(string password) {
    bool bigLetter(false), smallLetter(false), number(false), length(false);
            if (password.length() > 3 && password.length() < 25)
                length = true;
        for (int i=0; i<password.size(); i++) {
            if (password[i] >= 'a' && password[i] <= 'z')
                smallLetter = true;
            else if (password[i] >= 'A' && password[i] <= 'Z')
                bigLetter = true;
            else if (password[i] >= 48 && password[i] <= 57) // ASCII CODE FOR NUMBERS
                number = true;
        }
return (bigLetter && smallLetter && number && length);
}

bool checkUsername (string username) {
        bool startLetter(false), length(false);
        if (username[0] >= 'A' && username[0] <= 'Z')
            startLetter = true;
        if (username.length() > 3 && username.length() < 25)
            length = true;
return (startLetter && length);
}

bool isValidUsernameAndPassword (string username, string pass) {
return (checkUsername(username) && checkPassword(pass));
}

void Account::incijalizirajPaket(char znak) {
    switch(znak) {
case 'P':
    paket = "Premium";
    deposit-=10;
break;
case 'T':
    paket = "Trend";
    deposit-=8;
break;
case 'N':
    paket = "Normal";
    deposit-=5;
break;
    }
}

Account::Account (string name, string surname, string username, string password, double deposit, char znak, string currency) {
    if (!isValidUsernameAndPassword(username, password) || deposit < 10 || !checkCurrency(currency))
        throw range_error ("Greska pri kreiranju racuna, provjerite podatke!");
    this->name = name; this->surname = surname; this->deposit = deposit;
    this->username = username; this->password = password; this->currency = currency;
    incijalizirajPaket(znak);
}

Account& Account::raiseAmount(double amount, string currency) {
    if (amount > deposit)
        throw logic_error("Nemate dovoljno sredstava!");
deposit-=amount;
    cout << "Uspjesno vam je skinuto " << amount << currency <<endl;
return *this;
}

Account& Account::addAmount(double amount, string currency) {
    if (amount < 0)
        throw logic_error("Pogresna svota!");
    cout << "Uspjesno ste stavili " << amount << currency <<endl;
    deposit+=amount;
return *this;
}

Account& Account::changePassword(string newPass) {
    if (!checkPassword(newPass)) {
        cout << "Nepravilan password!";
        return *this;
    }
    this->password = newPass;
    cout << "Uspjesno ste promjenili password!" <<endl;
}


void Account::aboutAcc() const {
    cout << "Ime: " << name <<endl;
    cout << "Prezime: " << surname <<endl;
    cout << "Paket: " << paket <<endl;
    cout << "Username: " << username <<endl;
}


void Account::seeTransactions() const {
    if (gotTransactions.size() == 0 && sentTransactions.size() == 0) {
        cout << "Nema dostupnih transakcija!" <<endl;
        return;
    }
    for (int i=0; i<gotTransactions.size(); i++)
        gotTransactions[i].getAbout();
    for (int i=0; i<sentTransactions.size(); i++)
        sentTransactions[i].getAbout();
}

Bank& Bank::createAccount(string name, string surname, string username, string password, double deposit, char znak, string currency) {
    try {
        accounts.push_back(make_shared<Account>(name, surname, username, password, deposit, znak, currency));
    }
    catch(range_error izuz) {
        cout << izuz.what();
    }
}

int Bank::getAccID(string user, string pass) const {
        int index = -1;
    for (int i=0; i<accounts.size(); i++) {
        if (accounts[i]->getUsername() == user && accounts[i]->getPassword() == pass) {
            index = i;
            break;
        }
    }
return index;
}

Bank& Bank::loginAccount(string username, string password, int &indexRacuna) {
    if (getAccID(username, password) == -1)
        throw logic_error ("Nisu validni podaci, transakcija odbijena!");
    indexRacuna = getAccID(username, password);
}

Bank& Bank::raiseMoney(string username, string password, double amount, string currency) {
        int index;
        string accountCurrency;
        Date d;
    try {
        loginAccount(username, password, index);
        accountCurrency = accounts[index]->getCurrency();
        convertValute(amount, currency, accountCurrency);
        accounts[index]->raiseAmount(amount, accountCurrency);
        accounts[index]->addTransaction(d, username, amount, 'S');
        }
    catch(logic_error except) {
        cout << except.what();
    return *this;
    }
}

Bank& Bank::addAmount(string username, string password, double amount, string currency) {
        int index;
        string accountCurrency;
            Date d;
    try {
        loginAccount(username, password, index);
        accountCurrency = accounts[index]->getCurrency();
        convertValute(amount, currency, accountCurrency);
        accounts[index]->addAmount(amount, accountCurrency);
        accounts[index]->addTransaction(d, username, amount, 'R');
    }
    catch(logic_error exc) {
        cout << exc.what();
    return *this;
    }
}

Bank& Bank::changePassword(string username, string oldPass, string newPass) {
        int index;
    try {
    loginAccount(username, oldPass, index);
    accounts[index]->changePassword(newPass);
    }
    catch(logic_error e) {
        cout << e.what();
    }
}



Bank& Bank::changeCurrency(string username, string pass, string wantedCurrency) {
        int index;
        string currency;
        double deposit;
    try {
        loginAccount(username, pass, index);
        deposit = accounts[index]->getDeposit();
        currency = accounts[index]->getCurrency();
        convertValute(deposit, currency, wantedCurrency);
        accounts[index]->setDeposit(deposit);
        accounts[index]->setCurrency(currency);
    }
    catch(logic_error exc) {
        cout << exc.what();
    }
}

void Bank::seeDeposit(string username, string pass)  {
    try {
            int index;
        loginAccount(username, pass, index);
        cout << "Trenutno stanje: " <<accounts[index]->getDeposit() << " " << accounts[index]->getCurrency() <<endl;
    }
    catch(logic_error exc) {
        cout << exc.what();
    }
}

void Bank::seeTransactions(string username, string pass) {
    try {
        int index;
    loginAccount(username, pass, index);
        accounts[index]->seeTransactions();
    }
    catch(logic_error exc) {
        cout << exc.what();
    }
}

bool Bank::getAcc(string username, int &index) {
    for (int i=0; i<accounts.size(); i++) {
        if (username == accounts[i]->getUsername()) {
            index = i;
            return true;
        }
    }
return false;
}

// IF CHARACTER = 'S' then it is sent transactions other is get transaction
Account& Account::addTransaction(Date d, string name, double dep, char character) {
    if (character == 'S') {
            cout << "Transakcija u poslane uspjesno dodana!" <<endl;
    sentTransactions.push_back(SentTransactions(d, name, dep));
    }
    else {
            cout << "Transakcija u primljene uspjesno dodana!" <<endl;
        gotTransactions.push_back(RecievedTransactions(d, name, dep));
    }
}


Bank& Bank::transferAmount(string username, string pass, string otherUsername, double deposit, string currency) {
        int firstAcc, secondAcc;
        string fAccCurrency, sAccCurrency;
        if(!getAcc(otherUsername, secondAcc)) {
            cout << "Account nije pronadjen!" <<endl;
            return *this;
        }
        try {
    loginAccount(username, pass, firstAcc);
    Date d;
    fAccCurrency = accounts[firstAcc]->getCurrency();
    sAccCurrency = accounts[secondAcc]->getCurrency();
        cout << "Account pronadjen, molimo pricekajte..." <<endl;
            convertValute(deposit, currency, accounts[firstAcc]->getCurrency());
            accounts[firstAcc]->raiseAmount(deposit, currency);
            accounts[firstAcc]->addTransaction(d, username, deposit, 'S');
            convertValute(deposit, currency, accounts[secondAcc]->getCurrency());
            accounts[secondAcc]->addAmount(deposit, currency);
            accounts[secondAcc]->addTransaction(d, username, deposit, 'G');
        }
        catch(logic_error exc) {
            cout << exc.what();
        }
}

void Bank::seeAccounts(string name) {
    for (int i=0; i<accounts.size(); i++) {
        if (name == accounts[i]->getName())
            accounts[i]->aboutAcc();
    }
}

void SeeOptions () {
    cout << "\n0-Izlaz\n1-Promjena passworda\n2-Uplati na racun\n3-Isplati sa racuna\n4-Transfer novca" <<endl;
    cout << "5-Pristupi transakcijama dostupnim\n6-Provjeri informacije za ime\n7-Stanje na racunu\n8-Promjeni valutu" <<endl;
}

void SeeAvailablePackets () {
    cout << "P-premium(10km mjesecno)\nT-trend(8-km mjesecno)\nN-normal(5-km mjesecno)" <<endl;
}

bool checkPaket (char znak) {
return (znak == 'P' || znak == 'T' || znak == 'N');
}

void EnterInformations (string &name, string &surname, string &username, string &password, string &currency, double &deposit, char &znak) {
    SeeAvailablePackets();
        cout << "Unesite vase ime: ";
         cin >> name;
        cout << "Unesite vase prezime: ";
         cin >> surname;
        cout << "Username(3-25 slova, prvo slovo mora biti veliko): ";
         cin >> username;
        cout << "Password(3-25 slova, kombinacija malih i velikih): ";
         cin >> password;
        cout << "Novac koji uplacujete: ";
         cin >> deposit;
        do {
        cout << "Valuta u kojoj placate(E,USD,KM): ";
         cin >> currency;
         }while(currency != "E" && currency != "USD" && currency != "KM");
        cout << "Paket(znak): ";
         cin >> znak;
    if (!isValidUsernameAndPassword(username, password) || deposit < 10 || !checkCurrency(currency) || !checkPaket(znak))
        throw range_error("Nisu validni podaci!");
}

void Login (string &username, string &pass) {
    cout << "Vas username: ";
     cin >> username;
    cout << "Vas password: ";
     cin >> pass;
}

int choiceAfter () {
        int choice;
        do {
    cout << "Da li zelite izaci(0) ili se vratiti u main menu(1): ";
        cin >> choice;
        }while(choice < 0 ||choice > 1);
return choice;
}

void enterPass (string &oldPass, string &newPass) {
    cout << "Vas stari password: ";
        cin >> oldPass;
    cout << "Vas novi password: ";
        cin >> newPass;
}

int mainMenu () {
        int choice;
    SeeOptions();
            cout << "Vas izbor: ";
             cin >> choice;
return choice;
}

void inputAmount (double &amount, string &currency) {
    cout << "Iznos: ";
        cin >> amount;
    cout << "Valuta: ";
        cin >> currency;
}

void inputTransferData (string &object, double &amount, string &currency) {
    cout << "Username kome uplacujete: ";
        cin >> object;
    inputAmount(amount, currency);
}

int main() {
Bank intesa;
        int index, option, choice, choiceAft;
    string username, oldPass, newPass, password, currency;
        double amount;
            do {
         do {
            cout << "Zelite li napraviti racun(1) ili prijaviti se na postojeci racun(2): ";
             cin >> option;
            }while(option < 1 || option > 2);
        while (option == 1) {
            system ("cls");
            try {
        string name, surname, username, password, currency; double deposit; char znak;
        EnterInformations(name, surname, username, password, currency, deposit, znak);
        intesa.createAccount(name, surname, username, password, deposit, znak, currency);
            cout << "Racun uspjesno kreiran!" <<endl;
        option++; // IF DATAS ARE ALLRIGHT I'LL LEAVE LOOP //
            }
        catch(range_error exc) {
            cout << exc.what();
            system("pause");
            option = 1;
            }
        }
            system ("cls");
    cout << "Dobrodosli u nasu banku!" <<endl;
        bool failed(true);
        int counter(3);
        do {
    try {
    cout << "Ostalo vam je " << counter << " pokusaja!" <<endl;
        Login(username, password);
    intesa.loginAccount(username, password, index);
        failed = false;
    }
    catch(logic_error exc) {
        cout << exc.what();
        counter--;
     }
        }while(counter != 0 && failed);
    if (counter == 0) {
        cout << "Nemate vise pokusaja!" <<endl;
        return 0;
    }
        choice = mainMenu();
    switch(choice) {
case 0:
    return 0;
case 1:
    enterPass(oldPass, newPass);
    intesa.changePassword(username, oldPass, newPass);
    choiceAft = choiceAfter();
break;
case 2:
        inputAmount(amount, currency);
    intesa.addAmount(username, password, amount, currency);
    choiceAft = choiceAfter();
break;
case 3:
        inputAmount(amount, currency);
    intesa.raiseMoney(username, password, amount, currency);
    choiceAft = choiceAfter();
break;
case 4:
    {
        string object; // KOME UPLACUJE NOVAC NEMAM BOLJI NAZIV HAHA //
            inputTransferData(object, amount, currency);
    intesa.transferAmount(username, password, object, amount, currency);
    }
    choiceAft = choiceAfter();
break;
case 5:
    intesa.seeTransactions(username, password);
    choiceAft = choiceAfter();
break;
case 6:
{
        string name;
        cout << "Unesite ime: ";
         cin >> name;
    intesa.seeAccounts(name);
}
  choiceAft = choiceAfter();
break;
case 7:
    intesa.seeDeposit(username, password);
    choiceAft = choiceAfter();
break;
case 8:
    {
        string wantedCurrency;
            cout << "U koju valutu mijenjate deposit: ";
                cin >> wantedCurrency;
    intesa.changeCurrency(username, password, wantedCurrency);
    }
break;
default:
        {
        cout << "Pogresna komanda!" <<endl;
        choiceAft = 1;
        }
    }
        }while(choiceAft == 1);


    return 0;
}
