#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

namespace fs = boost::filesystem;

inline bool isPackage(const std::string &filename)
{
 boost::regex packageRegExp("(Descriptor).([xX][mM][lL])");
 boost::smatch xResults;
 return boost::regex_search(filename, xResults, packageRegExp, boost::match_all);
}
inline bool isV2Package(const std::string &filename)
{
 boost::regex packageV2RegExp("(((TPlusDescriptor.)|(T3Descriptor.))([xX][mM][lL]))");
 boost::smatch xResults;
 return boost::regex_search(filename, xResults, packageV2RegExp, boost::match_all);
}

int main()
{

 std::vector<std::string> packages;
 std::vector<std::string> packagesV2;
 for (fs::recursive_directory_iterator it("D:\TEST\DUMMY_SDK"), end; it != end; ++it)
 {

   std::cout << *it << std::endl;


   std::string filename = it->path().string();
   boost::regex packageV2RegExp("(((TPlusDescriptor.)|(T3Descriptor.))([xX][mM][lL]))");
   boost::regex packageRegExp("(Descriptor).([xX][mM][lL])");
   boost::smatch xResults;

   if (isPackage(filename))
   {
    if (isV2Package(filename))
     packagesV2.push_back(filename);
    else
     packages.push_back(filename);

    std::cout << filename << std::endl;
   }

 }

 if (packages.empty() && packagesV2.empty())
  return 0;

 boost::filesystem::ofstream out("com.ingenico.ingedev.telium.core.prefs");
 constexpr auto fileHeader = R"(installedPackages=< xml version\="1.0" encoding\="UTF-8" standalone\="no" >\r\n<packages>\r\n)";
 constexpr auto package = R"(<package location\=")";
 constexpr auto packagev2 = R"(<packageV2 location\=")";
 constexpr auto packageEndTag = R"("/>\r\n)";
 constexpr auto fileFooter = R"(</packages>\r\n)";
 out << fileHeader;
 for (auto it : packages)
 {
  out << package << it << packageEndTag;
 }
 for (auto it : packagesV2)
 {
  out << packagev2 << it << packageEndTag;
 }
 out << fileFooter;

 return 0;
}
//int main(int argc, char** argv)
//{
//
//}