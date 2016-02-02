# Boost Preprocessor Tutorial #
Here we show how to
  * perform Variadic template simulation in C++03
  * create a container for various function types that are linked by name

## Code ##
based on [this article (warning - russian)](http://dsukhonin.blogspot.com/2009/11/boostpreprocessor.html) and [boost preprocessor docs](http://www.boost.org/doc/libs/1_48_0/libs/preprocessor/doc/index.html)

```

#include <boost/preprocessor.hpp>
#include <iostream>
#include <string>
#include <map>
#include <vector>

//Redefine this if you need to handle more than 10 types.
#define CF_MAX_MAP_TYPES_COUNT 10
//Redefine this if you need another class name
#define CF_MAP_CLASS_NAME my_map

struct EmptyType {  };

#define CF_FILL_TYPES_II(z, n, T) \
class BOOST_PP_CAT(T, n ) = EmptyType

#define CF_FILL_TYPES_I(z, n, T) \
class BOOST_PP_CAT(T, n ) 

#define CF_FILL_TYPES(z, n, T) \
	BOOST_PP_CAT(T, n )

#define CF_FILL_TYPES_WITH_EMPTY_CLASS(z, n, T) \
	EmptyType

#define CF_ADD_TYPES(z, n, T) \
	typedef BOOST_PP_CAT(T, n ) BOOST_PP_CAT(BOOST_PP_CAT(type_, n ), ;)

#define CF_ADD_MAPS(z, n, T) \
	std::map<std::string, BOOST_PP_CAT(BOOST_PP_CAT(type_, n ), *) >  \
	BOOST_PP_CAT(BOOST_PP_CAT(T , BOOST_PP_CAT(n , _var)), ;)  

#define CF_ADD_OPERATORS(z, n, T) \
	operator BOOST_PP_CAT(type_, n )*()  \
{return \
	BOOST_PP_CAT( p->	, BOOST_PP_CAT(T , BOOST_PP_CAT(n , BOOST_PP_CAT( _var, [n_]) )) ) ; \
}

#define CF_ADD_INSERTERS(z, n, T) \
	void insert(std::string name,  BOOST_PP_CAT(BOOST_PP_CAT(type_, n ), *) ptr)   \
{ \
	BOOST_PP_CAT(T , BOOST_PP_CAT(n , BOOST_PP_CAT( _var, [name]) )) = ptr; \
}

#define CF_ADD_CONTENT(name, number) \
	BOOST_PP_REPEAT(number, CF_ADD_TYPES, T)  \
	BOOST_PP_REPEAT(number, CF_ADD_MAPS, T)        \
	friend class apitemp;                                        \
public: \
	name(  BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(number, int meaningless, 42) ) {}\
class apitemp {  \
	std::string n_;  \
	BOOST_PP_CAT(name, *) p; \
public: \
	apitemp(std::string name_, BOOST_PP_CAT(name, *) parent)  \
	: n_(name_), p(parent) {} \
	BOOST_PP_REPEAT(number, CF_ADD_OPERATORS, T) \
}; \
	BOOST_PP_REPEAT(number, CF_ADD_INSERTERS, T) \
	apitemp operator[](std::string n_) {return apitemp(n_, this);} 


#define CF_DEFINE_MAP_STRUCT_BASE(name, number)                   \
	template <BOOST_PP_ENUM(number, CF_FILL_TYPES_II, T)>              \
class name {   \
	CF_ADD_CONTENT(name, number) \
};

#define CF_DEFINE_MAP_STRUCT_REPETETIVE(name, number, totall)                   \
	template < BOOST_PP_ENUM(number, CF_FILL_TYPES_I, T) >              \
class name< BOOST_PP_ENUM(number, CF_FILL_TYPES, T) , BOOST_PP_ENUM( BOOST_PP_SUB(totall, number) , CF_FILL_TYPES_WITH_EMPTY_CLASS, T) > {   \
	CF_ADD_CONTENT(name, number) \
};

#define CF_DEFINE_MAP_STRUCT(r, state) CF_DEFINE_MAP_STRUCT_BASE(CF_MAP_CLASS_NAME, BOOST_PP_TUPLE_ELEM(2, 0, state))

#define CF_DEFINE_MAP_STRUCT_SECOND_I(z, number, name) \
	CF_DEFINE_MAP_STRUCT_REPETETIVE(name , number, CF_MAX_MAP_TYPES_COUNT) 

#define CF_DEFINE_MAP_STRUCT(name, number) \
	CF_DEFINE_MAP_STRUCT_BASE(name, number) \
	BOOST_PP_REPEAT_FROM_TO(1, CF_MAX_MAP_TYPES_COUNT, CF_DEFINE_MAP_STRUCT_SECOND_I, name) 

CF_DEFINE_MAP_STRUCT(CF_MAP_CLASS_NAME, CF_MAX_MAP_TYPES_COUNT)


	int hello_world(std::string name, const int & number )
{
	name += "!";
	std::cout << "Hello, " << name << std::endl;
	return number;
}

int main() {
	my_map<int(std::string, const int &), int(int)> myMap;
	my_map<int(std::string, const int &)> myMap2;
	myMap2.insert("my_method_hello", &hello_world ); 
	myMap.insert("my_method_hello", &hello_world ); 
	int a = myMap["my_method_hello"]("Tim", 25);
	int b = myMap2["my_method_hello"]("Bob", 69);
	std::cout << a << " "<< b << std::endl;
	std::cin.get();
}

```

## Output ##
```

Hello, Tim!
Hello, Bob!
25 69

```

## Preprocessor generated code ##
simplified for case of `CF_MAX_MAP_TYPES_COUNT = 4`, beautified via [this tool](http://indentcode.net/)

```

template < class T0 = EmptyType, class T1 = EmptyType, class T2 = EmptyType, class T3 = EmptyType > class my_map {
    typedef T0 type_0;
    typedef T1 type_1;
    typedef T2 type_2;
    typedef T3 type_3;
    std::map < std::string, type_0 * >T0_var;
    std::map < std::string, type_1 * >T1_var;
    std::map < std::string, type_2 * >T2_var;
    std::map < std::string, type_3 * >T3_var;
    friend class apitemp;
  public:my_map(int meaningless0 = 42, int meaningless1 = 42, int meaningless2 = 42, int meaningless3 = 42) {
    } class apitemp {
	std::string n_;
	my_map *p;
      public: apitemp(std::string name_, my_map * parent):n_(name_), p(parent) {
	} operator  type_0 *() {
	    return p->T0_var[n_];
	}
	operator  type_1 *() {
	    return p->T1_var[n_];
	}
	operator  type_2 *() {
	    return p->T2_var[n_];
	}
	operator  type_3 *() {
	    return p->T3_var[n_];
	}
    };
    void insert(std::string name, type_0 * ptr) {
	T0_var[name] = ptr;
    } void insert(std::string name, type_1 * ptr) {
	T1_var[name] = ptr;
    } void insert(std::string name, type_2 * ptr) {
	T2_var[name] = ptr;
    } void insert(std::string name, type_3 * ptr) {
	T3_var[name] = ptr;
    } apitemp operator[] (std::string n_) {
	return apitemp(n_, this);
    }
};

template < class T0 > class my_map < T0, EmptyType, EmptyType, EmptyType > {
    typedef T0 type_0;
    std::map < std::string, type_0 * >T0_var;
    friend class apitemp;
  public:my_map(int meaningless0 = 42) {
    } class apitemp {
	std::string n_;
	my_map *p;
      public: apitemp(std::string name_, my_map * parent):n_(name_), p(parent) {
	} operator  type_0 *() {
	    return p->T0_var[n_];
	}
    };
    void insert(std::string name, type_0 * ptr) {
	T0_var[name] = ptr;
    } apitemp operator[] (std::string n_) {
	return apitemp(n_, this);
    }
};

template < class T0, class T1 > class my_map < T0, T1, EmptyType, EmptyType > {
    typedef T0 type_0;
    typedef T1 type_1;
    std::map < std::string, type_0 * >T0_var;
    std::map < std::string, type_1 * >T1_var;
    friend class apitemp;
  public:my_map(int meaningless0 = 42, int meaningless1 = 42) {
    } class apitemp {
	std::string n_;
	my_map *p;
      public: apitemp(std::string name_, my_map * parent):n_(name_), p(parent) {
	} operator  type_0 *() {
	    return p->T0_var[n_];
	}
	operator  type_1 *() {
	    return p->T1_var[n_];
	}
    };
    void insert(std::string name, type_0 * ptr) {
	T0_var[name] = ptr;
    } void insert(std::string name, type_1 * ptr) {
	T1_var[name] = ptr;
    } apitemp operator[] (std::string n_) {
	return apitemp(n_, this);
    }
};

template < class T0, class T1, class T2 > class my_map < T0, T1, T2, EmptyType > {
    typedef T0 type_0;
    typedef T1 type_1;
    typedef T2 type_2;
    std::map < std::string, type_0 * >T0_var;
    std::map < std::string, type_1 * >T1_var;
    std::map < std::string, type_2 * >T2_var;
    friend class apitemp;
  public:my_map(int meaningless0 = 42, int meaningless1 = 42, int meaningless2 = 42) {
    } class apitemp {
	std::string n_;
	my_map *p;
      public: apitemp(std::string name_, my_map * parent):n_(name_), p(parent) {
	} operator  type_0 *() {
	    return p->T0_var[n_];
	}
	operator  type_1 *() {
	    return p->T1_var[n_];
	}
	operator  type_2 *() {
	    return p->T2_var[n_];
	}
    };
    void insert(std::string name, type_0 * ptr) {
	T0_var[name] = ptr;
    } void insert(std::string name, type_1 * ptr) {
	T1_var[name] = ptr;
    } void insert(std::string name, type_2 * ptr) {
	T2_var[name] = ptr;
    } apitemp operator[] (std::string n_) {
	return apitemp(n_, this);
    }
};


```

## reflection \ introspection ##
Please note that we will not provide full-blown  reflection\introspection library. There already is one grate called
[camp](https://github.com/tegesoft/camp/) (tutorial [here](http://blog.mestan.fr/2009/09/02/c-introspection-library-and-portable-industrial-graphical-components-for-gpl-and-commercial-use/)) and also boost.Reflection... they both mostly aim at Classes.