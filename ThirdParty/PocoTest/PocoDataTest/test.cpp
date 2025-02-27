#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include <vector>
#include <iostream>
#include <map>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

/**
 * 参考：http://www.cyqdata.com/cnblogs/article-detail-25077
 * 这篇文章的末尾提出了一种方法，还不错
 */
struct Person
{
	std::string name;
	std::string address;
	int         age;
};

struct GasPump
{
	int id;
	std::string type;
	std::string factName;
	int absP;
	int weight;
	int length;
	int width;
	int height;
};

namespace Poco {
	namespace Data {

		template <>
		class TypeHandler<GasPump>
		{
		public:
			//bind是在使用use到一个数据结构(不是基本的int、string)的时候被调用
			static void bind(std::size_t pos, const GasPump& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
			{
				poco_assert_dbg (!pBinder.isNull());

				std::cout<<"bind................."<<std::endl;

				// the table is defined as Person (FirstName VARCHAR(30), lastName VARCHAR, SocialSecNr INTEGER(3))
				// Note that we advance pos by the number of columns the datatype uses! For string/int this is one.
				pos++;
				//TypeHandler<int>::bind(pos++, obj.id, pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.type, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.absP, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.weight, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.length, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.width, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.height, pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.factName, pBinder, dir);
			}

			static std::size_t size()
			{
				return 8; // we handle three columns of the Table!
			}

			//prepare是在使用prepareStatement的时候被调用
			static void prepare(std::size_t pos, const GasPump& obj, AbstractPreparator::Ptr pPrepare)
			{
				std::cout<<"prepare................."<<std::endl;
				poco_assert_dbg (!pPrepare.isNull());
				// the table is defined as Person (FirstName VARCHAR(30), lastName VARCHAR, SocialSecNr INTEGER(3))
				// Note that we advance pos by the number of columns the datatype uses! For string/int this is one.

				TypeHandler<int>::prepare(pos++, obj.id, pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.type, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.absP, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.weight, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.length, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.width, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.height, pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.factName, pPrepare);
			}

			static void extract(std::size_t pos, GasPump& obj, const GasPump& defVal, AbstractExtractor::Ptr pExt)
				/// obj will contain the result, defVal contains values we should use when one column is NULL
			{
				//extract是在使用into到一个数据结构(不是基本的int、string)的时候被调用
				std::cout<<"extract................."<<std::endl;

				poco_assert_dbg (!pExt.isNull());

				TypeHandler<int>::extract(pos++, obj.id, defVal.id, pExt);
				TypeHandler<std::string>::extract(pos++, obj.type, defVal.type,pExt);
				TypeHandler<int>::extract(pos++, obj.absP, defVal.absP, pExt);
				TypeHandler<int>::extract(pos++, obj.weight, defVal.weight, pExt);
				TypeHandler<int>::extract(pos++, obj.length, defVal.length, pExt);
				TypeHandler<int>::extract(pos++, obj.width, defVal.width, pExt);
				TypeHandler<int>::extract(pos++, obj.height, defVal.height, pExt);
				TypeHandler<std::string>::extract(pos++, obj.factName, defVal.factName, pExt);
			}

		private:
			TypeHandler();
			~TypeHandler();
			TypeHandler(const TypeHandler&);
			TypeHandler& operator=(const TypeHandler&);
		};
	} } // namespace Poco::Data

void InitDbSystem()
{
	Poco::Data::SQLite::Connector::registerConnector();
}

void ShutDownDbSystem()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}

void test1()
{
	InitDbSystem();

	// create a session
	Session session("SQLite", "sample.db");

	// drop sample table, if it exists
	session << "DROP TABLE IF EXISTS Person", now;

	// (re)create table
	session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3))", now;

	// insert some rows
	Person person = 
	{
		"Bart Simpson",
		"Springfield",
		12
	};

	Statement insert(session);
	insert << "INSERT INTO Person VALUES(?, ?, ?)",
		use(person.name),
		use(person.address),
		use(person.age);

	std::string xx = insert.toString();
	insert.execute();

	person.name    = "Lisa Simpson";
	person.address = "Springfield";
	person.age     = 10;

	insert.execute();

	// a simple query
	Statement select(session);
	select << "SELECT Name, Address, Age FROM Person",
		into(person.name),
		into(person.address),
		into(person.age),
		range(0, 1); //  iterate over result set one row at a time

	while (!select.done())
	{
		select.execute();
		std::cout << person.name << " " << person.address << " " << person.age << std::endl;
	}

	ShutDownDbSystem();
}

void test2()
{
	InitDbSystem();

	// create a session
	Session session("SQLite", "test_pump2.db");

	// drop sample table, if it exists
	session << "DROP TABLE IF EXISTS TypeTable", now;

	// (re)create table
	session << "CREATE TABLE TypeTable ([id] INTEGER PRIMARY KEY AUTOINCREMENT, [type] NVARCHAR(20), [absP] INTEGER, [weight] INTEGER, [length] INTEGER,[weidth] INTEGER,[heigth] INTEGER,[factoryName] NVARCHAR(100))", now;
	
	GasPump pump;
	// insert some rows
	pump.type="2BEA-101-0";pump.absP=33;pump.weight=90;pump.length=638;pump.width=375;pump.height=355;pump.factName="淄博水环真空泵厂有限公司";

	Statement obj_insert(session);
	obj_insert << "INSERT INTO TypeTable VALUES(?, ?, ?, ?, ?, ?, ?, ?)", use(pump), now;

	std::cout<<"11111--------------------------------------------111111"<<std::endl;

	Statement insert(session);
	insert << "INSERT INTO TypeTable(type,absP,weight,length,weidth,heigth,factoryName) VALUES(?, ?, ?, ?, ?, ?, ?)",
		use(pump.type),
		use(pump.absP),
		use(pump.weight),
		use(pump.length),
		use(pump.width),
		use(pump.height),
		use(pump.factName);

	insert.execute();

	std::cout<<"2222--------------------------------------------2222"<<std::endl;

	pump.type="2BEA-101-0";pump.absP=33;pump.weight=110;pump.length=741;pump.width=375;pump.height=355;pump.factName="淄博水环真空泵厂有限公司";
	insert.execute();

	std::vector<int> ids;
	Statement select(session);
	select << "select id from TypeTable", into(ids), now;

	int id=0;
	select << "select id from TypeTable",into(id),range(0,1);
	while (!select.done())
	{
		select.execute();
		std::cout << id<<std::endl;
	}

	std::vector<std::string> types;
	Statement select2(session);
	select2 << "select type from TypeTable",into(types),now;

	std::cout<<"3333--------------------------------------------333"<<std::endl;

	std::vector<GasPump> pumps;
	Statement select3(session);
	select3 << "select * from TypeTable",into(pumps),now;

	std::cout<<"444--------------------------------------------444"<<std::endl;

	std::cout<<pumps[0].id<<std::endl;
	std::cout<<pumps[1].id<<std::endl;
	int aa=10;

	ShutDownDbSystem();
}

#include "PumpDatabase.h"

void test_dbHelper()
{
	DBHelper db("SQLite", "test_pump.db");
	db.createPumpTypeTable();

	PumpType pump;
	pump.type="2BEA-101-0";	pump.absP=33;pump.weight=110;pump.length=741;pump.width=375;pump.height=355;pump.factName="淄博水环真空泵厂有限公司";
	db.insertPumpType(pump);

	pump.type="2BEA-101-0";pump.absP=33;pump.weight=90;	pump.length=638;pump.width=375;	pump.height=355;pump.factName="淄博水环真空泵厂有限公司";
	db.insertPumpType(pump);

	int id = -1;
	db.getLastPumpTypeId(id);
	std::cout<<"最后插入的id是"<<id<<std::endl;

	PumpTypeTable tbls;
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	db.insertPumpTypeTable(tbls);

	id = -1;
	db.getLastPumpTypeId(id);
	std::cout<<"最后插入的id是"<<id<<std::endl;

	tbls.clear();
	db.getPumpTypeTable(tbls, "weight=90");
	std::cout<<"泵类型个数:"<<tbls.size()<<std::endl;

	PumpType new_pump = tbls[2];
	new_pump.id = 3; new_pump.absP=30335;
	db.updatePumpType(new_pump);

	//删除id=4的所有行
	db.delPumpType(4);

	tbls.clear();
	db.getPumpTypeTable(tbls);
	std::cout<<"泵类型个数:"<<tbls.size()<<std::endl;

	PumpType pump3;
	db.getPumpType(3, pump3);
	std::cout<<"获取第id=3的泵类型, id="<<pump3.id<<" absP="<<pump3.absP<<std::endl;
}

#include <fstream>
#include <vector>
typedef std::vector<std::string> StringArray;

void ReadTypeTable(StringArray& fields, PumpTypeTable& tbls)
{
	std::ifstream infile("pump_type.txt");
	if(!infile) return ;

	std::string field;
	for(int i=0;i<8;i++)
	{
		infile>>field;
		fields.push_back(field);
	}

	PumpType pump;
	while(!infile.eof())
	{
		infile>> pump.id >> pump.type >> pump.absP >> pump.weight >> pump.length >> pump.width >> pump.height >> pump.factName;
		tbls.push_back(pump);
		if(infile.fail()) break;
	}
	infile.close();
}

void ReadPropertyTable(StringArray& fields, PumpPropertyTable& tbls)
{
	std::ifstream infile("pump_property.txt");
	if(!infile) return ;

	std::string field;
	for(int i=0;i<7;i++)
	{
		infile>>field;
		fields.push_back(field);
	}

	PumpProperty pump;
	while(!infile.eof())
	{
		infile>> pump.id >> pump.catagory_id >> pump.speed >> pump.power >> pump.maxQ >> pump.maxP >> pump.absP;
		tbls.push_back(pump);
		if(infile.fail()) break;
	}
	infile.close();
}

void CreatePumpDB()
{
	StringArray type_fields;
	PumpTypeTable type_tbls;
	ReadTypeTable(type_fields, type_tbls);

	StringArray prop_fields;
	PumpPropertyTable prop_tbls;
	ReadPropertyTable(prop_fields, prop_tbls);

	//新建sqlite数据库
	DBHelper db("SQLite", "pump.db");
	//新建TypeTable表并插入数据
	db.createPumpTypeTable();
	db.insertPumpTypeTable(type_tbls);
	//新建PropertyTable表并插入数据
	db.createPumpPropertyTable();
	db.insertPumpPropertyTable(prop_tbls);
}

#include <Poco/TextConverter.h>
#include <poco/Windows1252Encoding.h>
#include <Poco/ASCIIEncoding.h>
#include <Poco/Latin1Encoding.h>
#include <Poco/UTF8Encoding.h>
#include <Poco/UTF16Encoding.h>

void test4()
{
	std::string latin1String("This is Latin-1 encoded text."); 
	std::string utf8String; 

	Poco::Latin1Encoding latin1;
	Poco::UTF8Encoding utf8; 

	Poco::TextConverter converter(latin1, utf8); 
	converter.convert(latin1String, utf8String); 

	std::cout << utf8String << std::endl; 
}

int main(int argc, char** argv)
{
	test1();
	test2();
	test_dbHelper();
	CreatePumpDB();

	return 0;
}