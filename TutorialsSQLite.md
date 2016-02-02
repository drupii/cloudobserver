# [sqlite3pp](http://code.google.com/p/sqlite3pp/) C++ Tutorial #
## Introduction ##
[SQLite](http://www.sqlite.org/) is a software library that implements a serverless SQL database engine. We use [sqlite3pp](http://code.google.com/p/sqlite3pp/) to get most of it from our C++ code.
Here is simple tutorial on using sqlite3pp.

## Getting started with SQLite and C++ ##
What we'll do:
  * We will create a DB file (if it does not exist) or open it.
  * Create a table in it (if it does not already exist)
  * Insert some values
  * Read out from DB some values.

_Note_ that SQLite DB engine does not usually throw `std::exeptions`, it just [returns SQLite error codes described here.](http://www.sqlite.org/c3ref/c_abort.html)

### Code ###
```

#include <iostream>
#include <sqlite3pp.h>

using namespace std;

int main(int argc, char* argv[])
{
	sqlite3pp::database db("test.db");
	try {

		cout <<	db.execute("CREATE TABLE IF NOT EXISTS users (email varchar(65) UNIQUE NOT NULL primary key, pass varchar(65))") << endl;
		cout <<	db.execute("INSERT INTO users (email, pass) VALUES ('AAAA', '1234')") << endl;

		sqlite3pp::transaction xct(db);

		sqlite3pp::command cmd(db, "INSERT INTO users (email, pass) VALUES (?, ?)");

		cout << cmd.bind(1, "BBBB") << endl;
		cout << cmd.bind(2, "1234") << endl;
		cout << cmd.execute() << endl;
		cout << cmd.reset() << endl;
		cmd.binder() << "CCCC" << "1234";
		cout << cmd.execute() << endl;
		xct.commit();
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
	}
	try 
	{
		sqlite3pp::transaction xct(db, true);

		sqlite3pp::query qry(db, "SELECT email, pass FROM users");

		cout << endl;

		for (int i = 0; i < qry.column_count(); ++i) {
			cout << qry.column_name(i) << "\t";
		}
		cout << endl;

		for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
			std::string email, pass;
			(*i).getter() >> email >> pass;
			cout << email << "\t" << pass << endl;
		}
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
	}
	cin.get();
}

```

### Output ###

So first time you run this code you will get nex output:
```
0
0
0
0
0
0
0

email   pass
AAAA    1234
BBBB    1234
CCCC    1234
```

As you can see no errors.

Bu if you will run it again from same location you will get:
```
0
19
0
0
1
19
1
column email is not unique

email   pass
AAAA    1234
BBBB    1234
CCCC    1234
```

Where
  * `0` is successful result
  * `1` is SQL error or missing database
  * `19` is abort due to constraint violation

### Conclusion ###
Please not only wrap your code into `try catch` but also inspect SQL return codes, only 0 is `OK` in most cases.


## SQLite PRAGMA example - DB  versioning ##
It is often required to monitor your current DB version and update it if needed. There is a way to store user defined DB version in SQLite db file. Well show how to use `pragma user_version` from C++ code

### Code ###
```
	
	#include <iostream>
	#include <boost/shared_ptr.hpp>
	#include <sqlite3pp.h>
	
	using namespace std;
	
	//note does not throw even if version was not set - returns 0 on SQLite 3
	int print_user_version(boost::shared_ptr<sqlite3pp::database> db)
	{
		sqlite3pp::query qry((*(db.get())), "PRAGMA user_version");
		for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
			int id;
			(*i).getter() >> id; 
			cout << id  <<endl;
			return id;
		}
		throw std::runtime_error("DB is not versioned!");
	}
	int main(int argc, char* argv[])
	{
		boost::shared_ptr<sqlite3pp::database>db(new sqlite3pp::database("test.db"));
		try{
			cout << print_user_version(db) << endl;
			cout << db->execute("PRAGMA user_version=10") << endl;
			cout << print_user_version(db) << endl;
			cout << db->execute("PRAGMA user_version='11'") << endl;
			cout << print_user_version(db) << endl;
		}
		catch (exception& ex) {
			cout << ex.what() << endl;
		}
		cin.get();
	}
	
```

### Output ###

First run (no existing DB file before app launch)

```
	0
	0
	0
	10
	10
	0
	11
	11
```
Next time you run your app youll see:
```
	11
	11
	0
	10
	10
	0
	11
	11
```

Note: Here 0 is SQLite return code in casre of command correct execution.

### Conclusion ###
Using such simple directive you can always see if your DB is up to date.