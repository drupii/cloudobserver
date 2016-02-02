# Boost Serialization Tutorial #

## As Text ##

Based on [official tutorial](http://www.boost.org/doc/libs/1_48_0/libs/serialization/doc/tutorial.html), shows simple nested classes serialisation demo:

### Code ###

```

	#include <boost/serialization/serialization.hpp>
	#include <boost/archive/text_oarchive.hpp>
	#include <boost/archive/text_iarchive.hpp>
	#include <string>
	#include <fstream>
	
	class A{
	private:
		friend class boost::serialization::access;
	
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & a_;
		}
	
		int a_;
	
	public:
		A(){ std::cout << "A constructed" << std::endl; }
		A(int a): a_(a) { std::cout << "A constructed with 'a' ==" << a << std::endl; }
		void print()
		{
			std::cout << "a == " << a_ << std::endl;
		}
	};
	class B{
	private:
		friend class boost::serialization::access;
	
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & b_;
		}
	
		std::string b_;
	public:
		B(){ std::cout << "B constructed" << std::endl; }
		B(std::string b): b_(b) { std::cout << "B constructed with 'b' ==" << b << std::endl; }
		void print()
		{
			std::cout << "b == " << b_ << std::endl;
		}
	};
	
	class C{
	private:
		friend class boost::serialization::access;
	
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & a_;
			ar & b_;
			ar & d_;
		}
	
		A a_;
		B b_;
		double d_;
	
	public:
		C(){  std::cout << "C constructed" << std::endl; }
		C(int a, std::string b, double d ): a_(a), b_(b), d_(d) { std::cout << "C constructed with 'd' == " << d << std::endl; }
		void print()
		{
			std::cout << "d == " << d_ << std::endl;
			a_.print();
			b_.print();
		}
	};
	
	int main() {
		// create and open a character archive for output
		std::ofstream ofs("filename");
	
		// create class instance
		std::cout << "create a class:" << std::endl;
		C c(15, "rock and roll", 25.8);
	
		// save data to archive
		std::cout << "encode:" << std::endl;
		{
			boost::archive::text_oarchive oa(ofs);
			// write class instance to archive
			oa << c;
			// archive and stream closed when destructors are called
		}
		std::cout << "encoded." << std::endl;
	
		C c_recreated;
		std::cout << "print empty:" << std::endl;
		c_recreated.print();
		{
			// create and open an archive for input
			std::ifstream ifs("filename");
			boost::archive::text_iarchive ia(ifs);
			// read class state from archive
			ia >> c_recreated;
			// archive and stream closed when destructors are called
		}
		std::cout << "print filled:" << std::endl;
		c_recreated.print();
	
		std::cin.get();
	}

```

### Output ###

```

	create a class:
	A constructed with 'a' ==15
	B constructed with 'b' ==rock and roll
	C constructed with 'd' == 25.8
	encode:
	encoded.
	A constructed
	B constructed
	C constructed
	print empty:
	d == -9.25596e+061
	a == -858993460
	b ==
	print filled:
	d == 25.8
	a == 15
	b == rock and roll

```

### Outputed File ###
```
	
	22 serialization::archive 9 0 0 0 0 15 0 0 13 rock and roll 25.800000000000001
	
```

## As XML ##

### Code ###
```
	
	#include <fstream>
	#include <iostream>
	#include <map>
	
	#include <boost/foreach.hpp>
	#include <boost/shared_ptr.hpp>
	
	#include <boost/archive/xml_iarchive.hpp>
	#include <boost/archive/xml_oarchive.hpp>
	#include <boost/serialization/map.hpp>
	#include <boost/serialization/shared_ptr.hpp>
	
	class A{
	private:
		friend class boost::serialization::access;
	
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::make_nvp("A", a_);
		}
	
		int a_;
	
	public:
		A(){ std::cout << "A constructed" << std::endl; }
		A(int a): a_(a) { std::cout << "A constructed with 'a' ==" << a << std::endl; }
		void print()
		{
			std::cout << "a == " << a_ << std::endl;
		}
	};
	
	class B{
	private:
		friend class boost::serialization::access;
	
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::make_nvp("B", b_);
		}
	
		std::string b_;
	public:
		B(){ std::cout << "B constructed" << std::endl; }
		B(std::string b): b_(b) { std::cout << "B constructed with 'b' ==" << b << std::endl; }
		void print()
		{
			std::cout << "b == " << b_ << std::endl;
		}
	};
	
	class C{
	private:
		friend class boost::serialization::access;
	
		double d_;
	
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::make_nvp("Map_A", map_a);
			ar & boost::serialization::make_nvp("Map_B", map_b);
			ar & boost::serialization::make_nvp("D", d_);
		}
	
		std::map<std::string, A*> map_a;
		std::map<std::string, boost::shared_ptr<B> > map_b;
	public:
	
		C(){  std::cout << "C constructed" << std::endl; }
		C(double d):  d_(d) {
			A* a_ = new A(1);
			map_a["hello"]= a_;
			A* a2_ = new A(2); 
			map_a["world"]= a2_;
	
			boost::shared_ptr<B>  b_ = boost::shared_ptr<B> ( new B("one"));
			map_b["hello"]= b_;
				boost::shared_ptr<B>  b2_ = boost::shared_ptr<B> ( new B("two"));
				map_b["world"]= b2_;
	
			std::cout << "C constructed with 'd' == " << d_ << std::endl;
		}
		void print()
		{
			std::cout << "d == " << d_ << std::endl;
	
			typedef std::map<std::string, A*> map_type_A;
			BOOST_FOREACH(const  map_type_A::value_type& pair, map_a )
			{
				pair.second->print();
			}
	
			typedef std::map<std::string, boost::shared_ptr<B> > map_type_B;
			BOOST_FOREACH(const  map_type_B::value_type& pair, map_b )
			{
				pair.second->print();
			}
		}
	};
	
	int main() {
		std::ofstream ofs("filename.xml");
	
		// create class instance
		std::cout << "create a class:" << std::endl;
		C c(25.8);
	
		// save data to archive
		std::cout << "encode:" << std::endl;
		{
			boost::archive::xml_oarchive oa(ofs,  boost::archive::no_header);
			// write class instance to archive
			oa << BOOST_SERIALIZATION_NVP(c);
			// archive and stream closed when destructors are called
		}
		std::cout << "encoded." << std::endl;
	
		C c_recreated;
		std::cout << "print empty:" << std::endl;
		c_recreated.print();
		{
			// create and open an archive for input
			std::ifstream ifs("filename.xml");
			boost::archive::xml_iarchive ia(ifs,  boost::archive::no_header );
			// read class state from archive
			ia >> BOOST_SERIALIZATION_NVP(c_recreated);
			// archive and stream closed when destructors are called
		}
		std::cout << "print filled:" << std::endl;
		c_recreated.print();
	
		std::cin.get();
	
		return 0;
	}
	
```

### Output ###
```
	
	create a class:
	A constructed with 'a' ==1
	A constructed with 'a' ==2
	B constructed with 'b' ==one
	B constructed with 'b' ==two
	C constructed with 'd' == 25.8
	encode:
	encoded.
	C constructed
	print empty:
	d == -9.25596e+061
	A constructed
	A constructed
	B constructed
	B constructed
	print filled:
	d == 25.8
	a == 1
	a == 2
	b == one
	b == two
	
```

### Outputed File ###
```
	
		<c class_id="0" tracking_level="0" version="0">
		<Map_A class_id="1" tracking_level="0" version="0">
			<count>2</count>
			<item_version>0</item_version>
			<item class_id="2" tracking_level="0" version="0">
				<first>hello</first>
				<second class_id="3" tracking_level="1" version="0" object_id="_0">
					<A>1</A>
				</second>
			</item>
			<item>
				<first>world</first>
				<second class_id_reference="3" object_id="_1">
					<A>2</A>
				</second>
			</item>
		</Map_A>
		<Map_B class_id="4" tracking_level="0" version="0">
			<count>2</count>
			<item_version>0</item_version>
			<item class_id="5" tracking_level="0" version="0">
				<first>hello</first>
				<second class_id="6" tracking_level="0" version="1">
					<px class_id="7" tracking_level="1" version="0" object_id="_2">
						<B>one</B>
					</px>
				</second>
			</item>
			<item>
				<first>world</first>
				<second>
					<px class_id_reference="7" object_id="_3">
						<B>two</B>
					</px>
				</second>
			</item>
		</Map_B>
		<D>25.800000000000001</D>
	</c>
	
```