研究反射时，研究了以下内容：
+ 反射的相关概念：元信息、支持库。
+ Java反射调研：支持哪些功能、接口设计。
+ C++反射库调研：支持哪些功能、接口设计、核心代码实现。
+ 反射在DCE7.0的现有应用，未来可扩展项。


研究时，还参照ponder反射库，参考其核心代码，实现了一个简易的C++反射库。细节在另一篇文档中介绍。
***
# 1 反射
反射功能依赖于元信息和支持库。支持库使用元信息来实现各种功能。

## 1.1 元信息
元信息保存的是类的各种信息，包括：
+ 类成员
  + 本类的成员
  + 父类的成员
+ 类方法
  + 普通方法
  + 构造
  + 析构

像Java、go这种语言层面支持反射，编译时就会把元信息保存起来。
C++编译器不会保存元信息，C++23都没有计划支持反射，等语言支持至少要c++26（2026年）。

C++可以借助第三方反射库来实现，注册保存元信息。
  
***
## 1.2 支持库
有了元信息，还需要支持库才能实现反射功能，例如Java里的java.lang.reflect包。Go的reflect包。

C++本身也是支持的，例如类型萃取，获取运行时类型，使用支持库是为了方便的使用元信息，提供统一接口。例如通过名字查找Class、通过名字修改类成员、通过名字调用类方法。



***
## 1.3 反射的功能
按照反射库功能的完善性，可以分为两类：类成员反射、类方法反射。
***
### 1.3.1 类成员反射
可以实现的应用：
+ 序列化、反序列化。
+ ORM



### 1.3.2 类方法反射
可以实现的应用：
+ RPC
+ 脚本语言（动态创建对象，调用对象方法）
+ 工厂模式
***
# 2 Java反射调研
为了防止只调研几个C++反射库被限制住思维，产生盲人摸象的情况。所以调研了Java和Go的反射及其接口设计。
因为Java与C++更像一些，所以本章主要介绍Java反射。

调研Java的反射后，再对照C++比较有名的ponder、rttr反射库。发现接口很类似，类的设计也很类似。从功能来看，Java能实现的，C++反射库基本都能实现。
***
## 2.1 元信息
java中每个类都有一个Class对象，每当编译一个类就会产生一个对应Class对象并且这个Class对象会被保存在同名.class文件里。无需手动注册。
***
## 2.2 对象
Java所有类都继承于Object，可以通过Class创建出来。
***
## 2.3 支持库类分层设计
Java 反射的主要组成部分有4个：Class、Field、Constructor、Method。

+ Class：类的元信息。也是Field、Constructor、Method的容器。
+ Field：类的成员。
+ Constructor：类的构造方法。
+ Method：类的方法。
+ Object：所有类的基类。
***
## 2.4 Class
### 2.4.1 Class获取
Class里包含了Field、Constructor、Method。还包含父类的一些信息。

使用反射的第一步就是获取Class对象。Java支持3种获取Class的方法：
+ 类型
```c++{.line-numbers}
Class cls = rmp_recv.class;
```

+ 类实例

```c++{.line-numbers}
rmp_recv sp = new rmp_recv();
Class cls = sp.getClass();
```
+ 类名字符串
```c++{.line-numbers}
Class cls = Class.forName("plug.rmp_recv");
```

### 2.4.2 Class应用
通过Class，可以创建类实例，例如经典的工厂模式。不用写一堆if else了。

```c++{.line-numbers}
Class cls = Class.forName("class_name");
plug_base plug = (plug_base)cls.newInstance(para1,para2);
```
***
## 2.5 Field
类里的每个成员对应一个Field。Class包含Field列表，可以通过Fi名字获取Field。通过Field可以读、写类成员。

### 2.5.1 读写类成员
1、获取类的Class对象
2、通过类成员的名字，获取类成员对应的Field对象
3、通过Field对象进行读写。

```c++{.line-numbers}
rmp_recv plug = new rmp_recv(123);  // 创建对象

Class cls = Class.forName("plug.rmp_recv");     // 获取Class
Field field = cls.getField("plug_id");          // 获取Field

int id = field.get(plug);   // 读
field.set(plug, 999);       // 写
```

### 2.5.2 遍历Field
Class提供了枚举Field的能力，为序列化反序列化提供了支持。

### 2.5.3 private类成员的访问
如果Field是private的，可以调用setAccessible获取访问权限。很方便，但是也破坏了设计初衷。
***
## 2.6 Method
Java调用类函数，有如下几步：
1、获取类的Class对象
2、通过函数名字，获取类成员对应的Method对象
3、通过Method对象进行调用
```c++{.line-numbers}
rmp_recv plug = new rmp_recv(123);  // 创建对象

Class cls = Class.forName("plug.rmp_recv");            // 获取Class
Method m = cls.getMethod("set_replay_point");          // 获取Method

m.invoke(plug,50000);               // 调用
```


### 2.6.1 private类方法的调用
同样可以调用setAccessible获取访问权限。
***

# 3 C++反射库调研
## 3.1 反射库的分类
### 3.1.1 功能
+ 类成员反射
    可以实现序列化、反序列化、ORM等功能。
+ 类成员反射+类方法反射
    可以实现上面功能外，还可以动态创建对象、RPC、实现脚本语言。

### 3.1.2 是否侵入性
+ 侵入式
  侵入式的反射库，需要再原有类中添加代码，主要是为了声名友元，好反射private成员、方法。
+ 非侵入式
  无需在原有类中添加任何代码，在外面注册元信息即可。不过无法反射private成员、方法。


### 3.1.3 无需注册元信息
https://cista.rocks/
大部分反射库都需要注册元信息，这个反射库不需要注册，就可以进行序列化和反序列化。利用的是c++的magic get方法，需要c—++17的支持。

优点、缺点都很明显：
+ 优点：无需注册元信息，使用方便。
+ 缺点：不能打印类成员的名字，也不能反射方法。使用限制：类必须支持用初始化列表构造（XX x={1,2,"xx"}）。

这个库比较有意思，不过实用价值不高。

***
## 3.2 调研的三方库
调研时，主要研究了ponder和rttr。属于功能完备的动态反射库。

+ ponder
地址：https://github.com/billyquith/ponder
star数：546
最后一次提交：2 years ago
+ rttr
地址：https://github.com/rttrorg/rttr
star数：2300
最后一次提交：11 months ago
***
### 3.2.1 反射功能、接口设计
ponder和rttr支持的功能与接口设计都很像，都支持类成员、类方法、构造反射。而且与Java的接口也很像，不同点主要有如下方面。

### 3.2.2 private 类成员、类方法访问

**private类成员**
+ Java
  + 通过setAccessible获取访问权限。
+ ponder、rttr
  + 侵入式，声名友元。
  + 非侵入式，注册get、set函数变相访问。

**private类成员**
+ Java
  + 通过setAccessible获取访问权限。
+ ponder、rttr
  + 侵入式，声名友元。

### 3.2.3 元信息的注册
+ Java：无需手动注册，语言支持。
+ C++第三方实现，需要手写注册代码。
***
## 3.3 反射功能实现：核心代码
调研时，研究了ponder库的代码。学习了ponder库对于类成员、类方法、构造函数的反射核心实现。

学些时参照ponder库，实现了简易的C++反射库（核心功能代码300行），下面简要介绍一下核心功能的实现，使用示例、实现细节请看另一篇。
***
### 3.3.1 类成员反射

类成员反射时，用到了一个不太常用的c++用法：
```c++{.line-numbers}
class X
{
    int pro1_;
};
void test1()
{
    X x{100};
    int X::* p1 = &X::pro1_;// 关键的！！！

    printf("pro1=%d\n", x.*p1);// get
    x.*p1 = 888;// set
}
```
上面关键的一行,保存了类成员的类型（int）、所属的类（X），在类中的偏移(p1的值)。可以利用它来访问类成员。

实现时，利用模板来保存类型和所属的类，并通过继承的方式实现类型擦除，类成员的名字、类成员信息用map存储。
```c++{.line-numbers}
class proper
{
    //。。。
};
template <class C, typename F>
class proper_impl : public proper
{
    //。。。
    F C::* pro_;
};
```
***
### 3.3.2 类构造函数反射
类构造函数反射时，核心是利用类模板生成对应的new 代码：
```c++{.line-numbers}
  return new C(args...)
```

实现时，也是利用模板保存类的类型、构造函数参数，利用继承实现类型擦除。

```c++{.line-numbers}
class construct
{
public:
	template <typename... A>
	void* create(A... arg)
	{
		return create_ex(args(arg...));
	}
private:
	virtual void* create_ex(args arg) = 0;
};

template <typename C, typename... A>
class construct_impl :public construct
{
private:
	virtual void* create_ex(args arg);
};
```

**参数类型擦除**
参数传递时，利用vector<std::any>（上面的args类）进行类型擦除，可以传递任意数量、任意类型的参数。
ponder和rttr库分别实现了std::variant,应该是从boost那引用的。同样实现了类型擦除功能。
***

### 3.3.3 类方法反射
类方法反射与类成员反射类似，同样是利用模板保存类方法的信息，然后使用继承进行类型擦除。

实现时，也是用了上面的vector<std::any>实现参数的类型擦除。并借助类型萃取，萃取出返回值、参数、类等信息。
***

## 3.4 元信息的自动注册

如果能自动生成元信息注册代码，那么C++的反射功能基本就和Java、Go这种原生支持的差不多了。

### 3.4.1 libclang
libclang是一个库，使用它可以分析源码得到AST（Abstracted Syntax Tree抽象语言语法树），有了AST就相当于“读懂”源码了，很多IDE、代码静态扫描工具都是使用libclang来分析代码。

### 3.4.2 使用libclang生成元信息注册代码
https://github.com/chakaz/reflang

这个C++反射库，除了提供反射的支持库代码以外，还提供工具扫描代码生成元信息注册代码。这个工具就是使用libclang来实现的，进一步简化了反射库的使用。
***

# 4 反射的应用
## 4.1 现状
DCE 7.0中已经应用了很多反射。虽然没有使用第三方反射库，但是也实现了反射的功能。

+ 平台VM-函数类型萃取
业务调用平台接口注册回调函数时，平台VM会对回调函数进行类型萃取，动态获取函数的返回值、参数列表信息，进行校验。

+ 平台PLUG-对象动态创建
平台根据配置文件、动态的创建不同的插件。也属于反射的应用，可以使用反射库增强扩展性。

+ 流水解析工具
流水解析工具可以读取流水，根据不同的opcode打印每个包的详细字段。其中包的元信息是python小工具生成的代码，有点类似protocol buf。
***
## 4.2 可以利用反射进行的优化
使用C++反射后，可以增加平台代码的灵活性，降低代码复杂度。

### 4.2.1 类成员反射
可以使用反射库，结构体和json字符串进行转化，降低代码复杂度，例如：
+ 读配置文件：无需写大量解析代码，直接把json反序列化为结构体。
+ 通信：模块和CMS通信、模块内部通信都大量使用json格式，使用反射后就不用手写大量的转化代码了。
+ 日志打印：例如插件，可以定时把自身序列化为json，打印状态，无需手写打印日志代码。

### 4.2.2 类方法反射
+ 对构造函数进行反射
可以采用“工厂模式”创建插件、不同种类的vm。
***





