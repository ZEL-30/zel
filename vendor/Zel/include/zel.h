/// @file zel.h
/// @author ZEL (zel1362848545@gmail.com)
/// @brief 开发框架公用函数和类的声明文件
/// @version 0.1
/// @date 2022-12-16
/// 
/// @copyright Copyright (c) 2022 ZEL

#ifndef PUBLIC_H
#define PUBLIC_H

// C
#include <Windows.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

// C++
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

// 命名空间
using namespace std;

// 字符串操作相关的函数和类
///////////////////////////////////// /////////////////////////////////////

/// @brief 安全的strcpy函数
/// @param dest 目标字符串，不需要初始化，在STRCPY函数中会对它进行初始化
/// @param destlen 目标字符串dest占用内存的大小
/// @param src 原字符串
/// @return 目标字符串dest的地址
/// @warning 超出dest容量的内容将丢弃
char *STRCPY(char *dest, const size_t destlen, const char *src);


/// @brief 安全的strncpy函数
/// @param dest 目标字符串，不需要初始化，在STRNCPY函数中会对它进行初始化
/// @param destlen 目标字符串dest占用内存的大小
/// @param src 原字符串
/// @param n 待复制的字节数 
/// @return char* 目标字符串dest的地址
/// @warning 超出dest容量的内容将丢弃
char *STRNCPY(char *dest, const size_t destlen, const char *src, size_t n);


/// @brief 安全的strcat函数
/// @param dest 目标字符串
/// @param destlen 目标字符串dest占用内存的大小
/// @param src 待追加的字符串
/// @return 目标字符串dest的地址
/// @warning 超出dest容量的内容将丢弃
char *STRCAT(char *dest, const size_t destlen, const char *src);


/// @brief 安全的strncat函数
/// @param dest 目标字符串
/// @param destlen 目标字符串dest占用内存的大小
/// @param src 待追加的字符串
/// @param n 待追加的字节数
/// @return char* 目标字符串dest的地址
/// @warning 超出dest容量的内容将丢弃
char *STRNCAT(char *dest, const size_t destlen, const char *src, size_t n);


/// @brief 安全的sprintf函数
///        将可变参数(...)按照fmt描述的格式输出到dest字符串中
/// @param dest 输出字符串，不需要初始化，在SPRINTF函数中会对它进行初始化
/// @param destlen 输出字符串dest占用内存的大小，如果格式化后的字符串长度大于destlen-1，后面的内容将丢弃
/// @param fmt 格式控制描述
/// @param ... 填充到格式控制描述fmt中的参数
/// @return int 格式化后的字符串的长度，程序员一般不关心返回值
int SPRINTF(char *dest, const size_t destlen, const char *fmt, ...);


/// @brief 安全的snprintf函数
///        将可变参数(...)按照fmt描述的格式输出到dest字符串中
/// @param dest 输出字符串，不需要初始化，在SNPRINTF函数中会对它进行初始化
/// @param destlen 输出字符串dest占用内存的大小，如果格式化后的字符串长度大于destlen-1，后面的内容将丢弃
/// @param n 把格式化后的字符串截取n-1存放到dest中，如果n>destlen-1，则取destlen-1
/// @param fmt 格式控制描述
/// @param ... 填充到格式控制描述fmt中的参数
/// @return int 格式化后的字符串的长度，程序员一般不关心返回值
/// @warning windows和linux平台下的snprintf函数的第三个参数n的用法略有不同。假设格式化后的字符串的长度
///          超过10,第三个参数n取值是10，那么，在windows平台下dest的长度将是10，linux平台下dest的长度却是9
int SNPRINTF(char *dest, const size_t destlen, size_t n, const char *fmt, ...);


/// @brief 删除字符串左边指定的字符
/// @param str 待处理的字符串
/// @param chr 需要删除的字符
void DeleteLChar(char *str, const char chr);


/// @brief 删除字符串右边指定的字符
/// @param str 待处理的字符串
/// @param chr 需要删除的字符
void DeleteRChar(char *str, const char chr);


/// @brief 删除字符串左右两边指定的字符
/// @param str 待处理的字符串
/// @param chr 需要删除的字符
void DeleteLRChar(char *str, const char chr);


/// @brief 把字符串中的小写字母转换成大写，忽略不是字母的字符
/// @param str 待转换的字符串，支持char[]和string两种类型
void ToUpper(char *str);
void ToUpper(string &str);


/// @brief 把字符串中的大写字母转换成小写，忽略不是字母的字符
/// @param str 待转换的字符串，支持char[]和string两种类型
void ToLower(char *str);
void ToLower(string &str);


/// @brief 字符串替换函数
///        在字符串str中，如果存在字符串str1，就替换为字符串str2
/// @param str 待处理的字符串
/// @param str1 旧的内容
/// @param str2 新的内容
/// @param bloop 是否循环执行替换
/// @warning 1. 如果str2比str1要长，替换后str会变长，所以必须保证str有足够的空间，否则内存会溢出;
///          2. 如果str2中包含了str1的内容，且bloop为true，这种做法存在逻辑错误，UpdateStr将什么也不做;
void UpdateStr(char *str, const char *str1, const char *str2,
               const bool bloop = true);


/// @brief 从一个字符串中提取出数字、符号和小数点，存放到另一个字符串中
/// @param src 原字符串
/// @param dest 目标字符串
/// @param bsigned 是否包括符号（+和-），true-包括；false-不包括
/// @param bdot 是否包括小数点的圆点符号，true-包括；false-不包括
void PickNumber(const char *src, char *dest, const bool bsigned,
                const bool bdot);


/// @brief 正则表达式，判断一个字符串是否匹配另一个字符串
/// @param str 需要判断的字符串，是精确表示的，如文件名"_public.cpp"
/// @param rules 匹配规则的表达式，用星号"*"代表任意字符串，多个表达式之间用半角的逗号分隔，如"*.h,*.cpp"
/// @return 成功 TRUE； 失败 FALSE
/// @warning 1. str参数不支持"*"，rules参数支持"*";
///          2. 函数在判断str是否匹配rules的时候，会忽略字母的大小写;
bool MatchStr(const string &str, const string &rules);
///////////////////////////////////// /////////////////////////////////////

///////////////////////////////////// /////////////////////////////////////

/// @brief CCmdStr类用于拆分有分隔符的字符串
/// 字符串的格式为：字段内容1+分隔符+字段内容2+分隔符+字段内容3+分隔符+...+字段内容n。
/// 例如："messi,10,striker,30,1.72,68.5,Barcelona"，这是足球运动员梅西的资料，包括姓名、
/// 球衣号码、场上位置、年龄、身高、体重和效力的俱乐部，字段之间用半角的逗号分隔。
class CCmdStr {

   public:
    vector<string> m_vCmdStr;  // 存放拆分后的字段内容。

    CCmdStr();  // 构造函数。
    CCmdStr(const string &buffer, const char *sepstr,
            const bool bdelspace = false);

    /// @brief 把字符串拆分到m_vCmdStr容器中
    /// @param buffer 待拆分的字符串
    /// @param sepstr buffer中采用的分隔符，注意，sepstr参数的数据类型不是字符，是字符串，请使用双引号
    /// @param bdelspace 拆分后是否删除字段内容前后的空格，true-删除；false-不删除，缺省不删除
    void SplitToCmd(const string &buffer, const char *sepstr,
                    const bool bdelspace = false);

    /// @brief 获取拆分后字段的个数，即
    /// @return m_vCmdStr容器的大小
    int CmdCount();

    /// @brief 从m_vCmdStr容器获取字段内容
    /// @param inum 字段的顺序号，类似数组的下标，从0开始
    /// @param value 传入变量的地址，用于存放字段内容
    /// @param ilen 字符串长度
    /// @return true-成功；如果inum的取值超出了m_vCmdStr容器的大小，返回失败
    bool GetValue(const int inum, char *value, const int ilen = 0);    // 字符串，ilen缺省值为0
    bool GetValue(const int inum, int *value);                         // int整数
    bool GetValue(const int inum, unsigned int *value);                // unsigned int整数
    bool GetValue(const int inum, long *value);                        // long整数
    bool GetValue(const int inum, unsigned long *value);               // unsigned long整数
    bool GetValue(const int inum, double *value);                      // 双精度double
    bool GetValue(const int inum, bool *value);                        // bool型

    ~CCmdStr();  // 析构函数。
};
///////////////////////////////////// /////////////////////////////////////


// xml格式的字符串的内容如下：
// <filename>/tmp/_public.h</filename><mtime>2020-01-01
// 12:20:35</mtime><size>18348</size>
// <filename>/tmp/_public.cpp</filename><mtime>2020-01-01
// 10:10:15</mtime><size>50945</size> 
///////////////////////////////////// /////////////////////////////////////

/// @brief 解析xml格式字符串的函数族
/// @param xmlbuffer 待解析的xml格式字符串
/// @param fieldname 字段的标签名
/// @param value 传入变量的地址，用于存放字段内容，支持bool、int、insigned int、long、unsigned long、double和char[]
/// @param ilen 
/// @return true-成功；如果fieldname参数指定的标签名不存在，返回失败
/// @warning 当value参数的数据类型为char[]时，必须保证value数组的内存足够，否则可能发生内存溢出的问题，也可以用ilen参数限定获取字段内容的长度，ilen的缺省值为0，表示不限长度
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, char *value,
                  const int ilen = 0);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, bool *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, int *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname,
                  unsigned int *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, long *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname,
                  unsigned long *value);
bool GetXMLBuffer(const char *xmlbuffer, const char *fieldname, double *value);
///////////////////////////////////// /////////////////////////////////////

// 取操作系统的时间。
///////////////////////////////////// /////////////////////////////////////
/*
  stime：用于存放获取到的时间字符串。
  timetvl：时间的偏移量，单位：秒，0是缺省值，表示当前时间，30表示当前时间30秒之后的时间点，-30表示当前时间30秒之前的时间点。
  fmt：输出时间的格式，fmt每部分的含义：yyyy-年份；mm-月份；dd-日期；hh24-小时；mi-分钟；ss-秒，
  缺省是"yyyy-mm-dd hh24:mi:ss"，目前支持以下格式：
  "yyyy-mm-dd hh24:mi:ss"
  "yyyymmddhh24miss"
  "yyyy-mm-dd"
  "yyyymmdd"
  "hh24:mi:ss"
  "hh24miss"
  "hh24:mi"
  "hh24mi"
  "hh24"
  "mi"
  注意：
    1）小时的表示方法是hh24，不是hh，这么做的目的是为了保持与数据库的时间表示方法一致；
    2）以上列出了常用的时间格式，如果不能满足你应用开发的需求，请修改源代码timetostr函数增加更多的格式支持；
    3）调用函数的时候，如果fmt与上述格式都匹配，stime的内容将为空。
    4）时间的年份是四位，其它的可能是一位和两位，如果不足两位，在前面补0。
*/
void LocalTime(char *stime, const char *fmt = 0, const int timetvl = 0);


/// @brief 把整数表示的时间转换为字符串表示的时间
/// @param ltime 整数表示的时间
/// @param stime 字符串表示的时间
/// @param fmt 输出字符串时间stime的格式，与LocalTime函数的fmt参数相同，如果fmt的格式不正确，stime将为空
void timetostr(const time_t ltime, char *stime, const char *fmt = 0);


/// @brief 把字符串表示的时间转换为整数表示的时间
/// @param stime 字符串表示的时间，格式不限，但一定要包括yyyymmddhh24miss，一个都不能少，顺序也不能变
/// @return time_t 整数表示的时间，如果stime的格式不正确，返回-1
time_t strtotime(const char *stime);


/// @brief 把字符串表示的时间加上一个偏移的秒数后得到一个新的字符串表示的时间
/// @param in_stime 输入的字符串格式的时间，格式不限，但一定要包括yyyymmddhh24miss，一个都不能少，顺序也不能变
/// @param out_stime 输出的字符串格式的时间
/// @param timetvl 需要偏移的秒数，正数往后偏移，负数往前偏移
/// @param fmt 输出字符串时间out_stime的格式，与LocalTime函数的fmt参数相同
/// @return true-成功，false-失败，如果返回失败，可以认为是in_stime的格式不正确
/// @warning in_stime和out_stime参数可以是同一个变量的地址，如果调用失败，out_stime的内容会清空
bool AddTime(const char *in_stime, char *out_stime, const int timetvl,
             const char *fmt = 0);
///////////////////////////////////// /////////////////////////////////////

///////////////////////////////////// /////////////////////////////////////

/// @brief 这是一个精确到微秒的计时器
class CTimer {

public:

	/// @brief 开始计时
    void Start();

    /// @brief 构造函数中会调用Start方法
    CTimer();

	/// @brief 计算已逝去的时间，单位：秒，小数点后面是微秒
	///        每调用一次本方法之后，自动调用Start方法重新开始计时
	/// @return double 逝去的时间，单位：秒，小数点后面是微秒 
    double Elapsed();

public:
    struct timeval m_start;  // 开始计时的时间。
    struct timeval m_end;    // 计时完成的时间。

};
///////////////////////////////////// /////////////////////////////////////

// 目录操作相关的类
///////////////////////////////////// /////////////////////////////////////

/// @brief 根据绝对路径的文件名或目录名逐级的创建目录
/// @param pathorfilename 绝对路径的文件名或目录名
/// @param bisfilename 说明pathorfilename的类型，true-pathorfilename是文件名，否则是目录名，缺省值为tru
/// @return true-成功，false-失败，如果返回失败，原因有大概有三种情况：
///         1. 权限不足；
///         2. pathorfilename参数不是合法的文件名或目录名；
///         3. 磁盘空间不足
bool MKDIR(const char *pathorfilename, bool bisfilename = true);


/// @brief 获取某目录及其子目录中的文件列表信息
class CDir {

public:
    /// @brief 构造函数
    CDir();

    /// @brief 初始化成员变量
    void initdata();

	/// @brief 设置文件时间的格式，支持"yyyy-mm-dd
	///	hh24:mi:ss"和"yyyymmddhh24miss"两种，缺省是前者 
	/// @param in_DateFMT 
    void SetDateFMT(const char *in_DateFMT);

    /// @brief 打开目录，获取目录中的文件列表信息，存放于m_vFileName容器中
    /// @param in_DirName 待打开的目录名，采用绝对路径，如/tmp/root
    /// @param in_MatchStr 待获取文件名的匹配规则，不匹配的文件被忽略，具体请参见开发框架的MatchStr函数
    /// @param in_MaxCount 获取文件的最大数量，缺省值为10000个
    /// @param bAndChild 是否打开各级子目录，缺省值为false-不打开子目录
    /// @param bSort 是否对获取到的文件列表（即m_vFileName容器中的内容）进行排序，缺省值为false-不排序
    /// @return true-成功，false-失败，如果in_DirName参数指定的目录不存在，OpenDir方法会创建该目录，如果创建失败，返回false，如果当前用户对in_DirName目录下的子目录没有读取权限也会返回false
    bool OpenDir(const char *in_DirName, const char *in_MatchStr,
                 const unsigned int in_MaxCount = 10000,
                 const bool bAndChild = false, bool bSort = false);

    // 这是一个递归函数，被OpenDir()的调用，在CDir类的外部不需要调用它。
    bool _OpenDir(const char *in_DirName, const char *in_MatchStr,
                  const unsigned int in_MaxCount, const bool bAndChild);

    // 从m_vFileName容器中获取一条记录（文件名），同时获取该文件的大小、修改时间等信息。
    // 调用OpenDir方法时，m_vFileName容器被清空，m_pos归零，每调用一次ReadDir方法m_pos加1。
    // 当m_pos小于m_vFileName.size()，返回true，否则返回false。
    bool ReadDir();

    ~CDir();  // 析构函数。

public:
    char m_DirName[301];         // 目录名，例如：/tmp/root。
    char m_FileName[301];        // 文件名，不包括目录名，例如：data.xml。
    char m_FullFileName[301];    // 文件全名，包括目录名，例如：/tmp/root/data.xml。
    int  m_FileSize;             // 文件的大小，单位：字节。
    char m_ModifyTime[21];       // 文件最后一次被修改的时间，即stat结构体的st_mtime成员。
    char m_CreateTime[21];       // 文件生成的时间，即stat结构体的st_ctime成员。
    char m_AccessTime[21];       // 文件最后一次被访问的时间，即stat结构体的st_atime成员。
    char m_DateFMT[25];          // 文件时间显示格式，由SetDateFMT方法设置。

    vector<string> m_vFileName;  // 存放OpenDir方法获取到的绝对路径文件名清单。
    int m_pos;                   // 已读取m_vFileName容器的位置，每调用一次ReadDir方法m_pos加1。

};
///////////////////////////////////// /////////////////////////////////////

// 文件操作相关的函数和类
///////////////////////////////////// /////////////////////////////////////

/// @brief  删除文件，类似Linux系统的rm命令
/// @param filename 待删除的文件名，建议采用绝对路径的文件名，例如/tmp/root/data.xml
/// @param times 执行删除文件的次数，缺省是1，建议不要超过3，从实际应用的经验看来，如果删除文件第1次不成功，再尝试2次是可以的，更多次就意义不大了。还有，如果执行删除失败，usleep(100000)后再重试
/// @return true true-成功；false-失败，失败的主要原因是权限不足
/// @note 在应用开发中，可以用REMOVE函数代替remove库函数
bool REMOVE(const char *filename, const int times = 1);


/// @brief 重命名文件，类似Linux系统的mv命令
/// @param srcfilename 原文件名，建议采用绝对路径的文件名
/// @param dstfilename 目标文件名，建议采用绝对路径的文件名
/// @param times 执行重命名文件的次数，缺省是1，建议不要超过3，从实际应用的经验看来，如果重命名文件第1次不成功，再尝试2次是可以的，更多次就意义不大了。还有，如果执行重命名失败，usleep(100000)后再重试
/// @return true-成功；false-失败，失败的主要原因是权限不足或磁盘空间不够，如果原文件和目标文件不在同一个磁盘分区，重命名也可能失败
/// @warning 在重命名文件之前，会自动创建dstfilename参数中包含的目录
/// @note 在应用开发中，可以用RENAME函数代替rename库函数
bool RENAME(const char *srcfilename, const char *dstfilename, const int times = 1);


/// @brief 复制文件，类似Linux系统的cp命令
/// @param srcfilename 原文件名，建议采用绝对路径的文件名
/// @param dstfilename 目标文件名，建议采用绝对路径的文件名
/// @return true-成功；false-失败，失败的主要原因是权限不足或磁盘空间不够
/// @warning 1. 在复制文件之前，会自动创建dstfilename参数中的目录名;
///          2. 复制文件的过程中，采用临时文件命名的方法，复制完成后再改名为dstfilename，避免中间状态的文件被读取;
///          3. 复制后的文件的时间与原文件相同，这一点与Linux系统cp命令不同;
bool COPY(const char *srcfilename, const char *dstfilename);


/// @brief 获取文件的大小
/// @param filename 待获取的文件名，建议采用绝对路径的文件名
/// @return int 如果文件不存在或没有访问权限，返回-1，成功返回文件的大小，单位是字节
int FileSize(const char *filename);


/// @brief 获取文件的时间
/// @param filename 待获取的文件名，建议采用绝对路径的文件名
/// @param mtime 用于存放文件的时间，即stat结构体的st_mtime
/// @param fmt 设置时间的输出格式，与LocalTime函数相同，但缺省是"yyyymmddhh24miss"
/// @return 如果文件不存在或没有访问权限，返回false，成功返回true
bool FileMTime(const char *filename, char *mtime, const char *fmt = 0);


/// @brief 重置文件的修改时间属性
/// @param filename 待重置的文件名，建议采用绝对路径的文件名
/// @param mtime 字符串表示的时间，格式不限，但一定要包括yyyymmddhh24miss，一个都不能少，顺序也不能变
/// @return true-成功；false-失败，失败的原因保存在errno中
bool UTime(const char *filename, const char *mtime);


/// @brief 打开文件
/// @param filename 文件名
/// @param mode 打开文件的模式 
/// @return 文件指针
/// @note 1. FOPEN函数调用fopen库函数打开文件，如果文件名中包含的目录不存在，就创建目录;
///       2. 在应用开发中，用FOPEN函数代替fopen库函数
FILE *FOPEN(const char *filename, const char *mode);


/// @brief 从文本文件中读取一行
/// @param fp 已打开的文件指针
/// @param buffer 用于存放读取的内容，buffer必须大于readsize+1，否则可能会造成读到的数据不完整或内存的溢出
/// @param readsize 本次打算读取的字节数，如果已经读取到了行结束标志，函数返回
/// @param endbz 行内容结束的标志，缺省为空，表示行内容以"\n"为结束标志
/// @return true-成功；false-失败，一般情况下，失败可以认为是文件已结束
bool FGETS(const FILE *fp, char *buffer, const int readsize, const char *endbz = 0);

// 文件操作类声明
class CFile {

public:

    /// @brief 构造函数
    CFile();

    /// @brief 判断文件是否已打开
    /// @return true-已打开；false-未打开
    bool IsOpened();

	/// @brief 专为重命名而打开文件
	/// @param filename 待打开的文件名，建议采用绝对路径的文件名
	/// @param openmode 打开文件的模式，与fopen库函数的打开模式相同
	/// @param bEnBuffer 是否启用缓冲，true-启用；false-不启用，缺省是启用
	/// @return true-成功；false-失败 
	/// @warning 如果待打开的文件的目录不存在，就会创建目录
    bool Open(const char *filename, const char *openmode, bool bEnBuffer = true);

	/// @brief 关闭文件指针，并删除文件
	/// @return true-成功；false-失败 
    bool CloseAndRemove();

	/// @brief 专为重命名而打开文件
	/// @param filename 待打开的文件名，建议采用绝对路径的文件名
	/// @param openmode 打开文件的模式，与fopen库函数的打开模式相同
	/// @param bEnBuffer 是否启用缓冲，true-启用；false-不启用，缺省是启用
	/// @return true-成功；false-失败 
	/// @warning OpenForRename打开的是filename后加".tmp"的临时文件，所以openmode只能是"a"、"a+"、"w"、"w+"
    bool OpenForRename(const char *filename, const char *openmode, bool bEnBuffer = true);

	/// @brief 关闭文件指针，并把OpenForRename方法打开的临时文件名重命名为filename
	/// @return true-成功；false-失败
    bool CloseAndRename();

	/// @brief 调用fprintf向文件写入数据，参数与fprintf库函数相同，但不需要传入文件指针
	/// @param fmt 
	/// @param ... 
    void Fprintf(const char *fmt, ...);

	/// @brief 从文件中读取以换行符"\n"结束的一行，类似fgets函数
	/// @param buffer 用于存放读取的内容，buffer必须大于readsize+1，否则可能会造成内存的溢出
	/// @param readsize 本次打算读取的字节数，如果已经读取到了结束标志"\n"，函数返回
	/// @param bdelcrt 是否删除行结束标志"\r"和"\n"，true-删除；false-不删除，缺省值是false
	/// @return true-成功；false-失败，一般情况下，失败可以认为是文件已结束
    bool Fgets(char *buffer, const int readsize, bool bdelcrt = false);

	/// @brief 从文件文件中读取一行
	/// @param buffer 用于存放读取的内容，buffer必须大于readsize+1，否则可能会造成读到的数据不完整或内存的溢出
	/// @param readsize 本次打算读取的字节数，如果已经读取到了结束标志，函数返回
	/// @param endbz 行内容结束的标志，缺省为空，表示行内容以"\n"为结束标志
	/// @return true-成功；false-失败，一般情况下，失败可以认为是文件已结束
    bool FFGETS(char *buffer, const int readsize, const char *endbz = 0);

	/// @brief 从文件中读取数据块
	/// @param ptr 用于存放读取的内容
	/// @param size 本次打算读取的字节数
	/// @return size_t 本次从文件中成功读取的字节数，如果文件未结束，返回值等于size，如果文件已结束，返回值为实际读取的字节数
    size_t Fread(void *ptr, size_t size);

	/// @brief 向文件中写入数据块
	/// @param ptr 待写入数据块的地址
	/// @param size 待写入数据块的字节数
	/// @return size_t 本次成功写入的字节数，如果磁盘空间足够，返回值等于size
    size_t Fwrite(const void *ptr, size_t size);

	/// @brief 关闭文件指针，如果存在临时文件，就删除它
    void Close();

	/// @brief 析构函数会调用Close方法 
    ~CFile();

private:

    FILE *m_fp;               // 文件指针
    bool m_bEnBuffer;         // 是否启用缓冲，true-启用；false-不启用，缺省是启用。
    char m_filename[301];     // 文件名，建议采用绝对路径的文件名。
    char m_filenametmp[301];  // 临时文件名，在m_filename后加".tmp"。

};
///////////////////////////////////// /////////////////////////////////////

// 以下是日志文件操作类
///////////////////////////////////// /////////////////////////////////////
// 日志文件操作类
class CLogFile {
  
public:

	/// @brief Construct a new CLogFile object
	/// @param MaxLogSize 最大日志文件的大小，单位M，缺省100M，最小为10M
    CLogFile(const long MaxLogSize = 100);

    /// @brief 打开日志文件
    /// @param filename 日志文件名，建议采用绝对路径，如果文件名中的目录不存在，就先创建目录
    /// @param openmode 日志文件的打开方式，与fopen库函数打开文件的方式相同，缺省值是"a+"
    /// @param bBackup 是否自动切换，true-切换，false-不切换，在多进程的服务程序中，如果多个进程共用一个日志文件，bBackup必须为false
    /// @param bEnBuffer 是否启用文件缓冲机制，true-启用，false-不启用，如果启用缓冲区，那么写进日志文件中的内容不会立即写入文件，缺省是不启用
    /// @return true-成功， false-失败
    bool Open(const char *filename, const char *openmode = 0, bool bBackup = true, bool bEnBuffer = false);

	/// @brief 如果日志文件大于m_MaxLogSize的值，就把当前的日志文件名改为历史日志文件名，再创建新的当前日志文件
	///        备份后的文件会在日志文件名后加上日期时间，如/tmp/log/filetodb.log.20200101123025
    /// @return true-成功， false-失败
	/// @warning 注意，在多进程的程序中，日志文件不可切换，多线的程序中，日志文件可以切换
    bool BackupLogFile();

    /// @brief 把内容写入日志文件
    /// @param fmt 可变参数，使用方法与printf库函数相同
    /// @return true-成功， false-失败
	/// @note Write方法会写入当前的时间，WriteEx方法不写时间
    bool Write(const char *fmt, ...);
    bool WriteEx(const char *fmt, ...);

	/// @brief 关闭日志文件
    void Close();

    ~CLogFile();  // 析构函数会调用Close方法


public:

    FILE *m_tracefp;            // 日志文件指针
    char m_filename[301];       // 日志文件名，建议采用绝对路径
    char m_openmode[11];        // 日志文件的打开方式，一般采用"a+"
    bool m_bEnBuffer;           // 写入日志时，是否启用操作系统的缓冲机制，缺省不启用
    bool m_bBackup;             // 是否自动切换，日志文件大小超过m_MaxLogSize将自动切换，缺省启用
    long m_MaxLogSize;          // 最大日志文件的大小，单位M，缺省100M
    pthread_spinlock_t   spin;  // 声明自旋锁，给日志文件类加锁是防止多线程操作日志文件时出现的线程安全问题

};
///////////////////////////////////// /////////////////////////////////////

// 参数文件操作类
///////////////////////////////////// /////////////////////////////////////
// CIniFile类操作的不是传统的ini文件，是xml格式的参数文件，例如：
/*
 * 文件名：hssms.xml
<?xml version="1.0" encoding="gbk" ?>
<root>
    <!-- 程序运行的日志文件名。 -->
    <logpath>/log/hssms</logpath>

    <!-- 数据库连接参数。 -->
    <connstr>hssms/smspwd@hssmszx</connstr>

    <!-- 数据文件存放的根目录。 -->
    <datapath>/data/hssms</datapath>

    <!-- 中心服务器的ip地址。 -->
    <serverip>192.168.1.1</serverip>

    <!-- 中心服务器的通讯端口。 -->
    <port>5058</port>

    <!-- 是否采用长连接，true-是；false-否。 -->
    <online>true</online>
</root>
*/

/// @brief 参数文件操作类
class CIniFile {
   
public:

    CIniFile();

	/// @brief 把参数文件的内容载入到m_xmlbuffer成员变量中
	/// @param filename 文件名
	/// @return true 
    bool LoadFile(const char *filename);

    /// @brief 获取参数的值
    /// @param fieldname 字段的标签名
    /// @param value 传入变量的地址，用于存放字段的值，支持bool、int、insigned int、long、unsigned long、double和char[]
    /// @return true-成功；false-失败
	/// @warning 当value参数的数据类型为char[]时，必须保证value的内存足够，否则可能发生内存溢出的问题, 也可以用ilen参数限定获取字段内容的长度，ilen的缺省值为0，表示不限定获取字段内容的长度
    bool GetValue(const char *fieldname, bool *value);
    bool GetValue(const char *fieldname, int *value);
    bool GetValue(const char *fieldname, unsigned int *value);
    bool GetValue(const char *fieldname, long *value);
    bool GetValue(const char *fieldname, unsigned long *value);
    bool GetValue(const char *fieldname, double *value);
    bool GetValue(const char *fieldname, char *value, const int ilen = 0);

public:
    string m_xmlbuffer;  // 存放参数文件全部的内容，由LoadFile方法载入。
};
///////////////////////////////////// /////////////////////////////////////

#endif