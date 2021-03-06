#pragma once
#include "../cni.hpp"
#include <fstream>
static cs::extension file_ext;
static cs::extension infile_ext;
static cs::extension outfile_ext;
static cs::extension_t infile_ext_shared=std::make_shared<cs::extension_holder>(&infile_ext);
static cs::extension_t outfile_ext_shared=std::make_shared<cs::extension_holder>(&outfile_ext);
namespace cs_impl {
	template<>cs::extension_t& get_ext<std::shared_ptr<std::ifstream>>()
	{
		return infile_ext_shared;
	}
	template<>cs::extension_t& get_ext<std::shared_ptr<std::ofstream>>()
	{
		return outfile_ext_shared;
	}
}
namespace file_cs_ext {
	using namespace cs;
	using infile=std::shared_ptr<std::ifstream>;
	using outfile=std::shared_ptr<std::ofstream>;
	enum class file_method {
		read,write
	};
	var open(const string& path,file_method method)
	{
		switch (method) {
		case file_method::read:
			return std::make_shared<std::ifstream>(path);
			break;
		case file_method::write:
			return std::make_shared<std::ofstream>(path);
			break;
		}
	}
	bool is_open(const infile& infs)
	{
		return infs->is_open();
	}
	bool eof(const infile& infs)
	{
		return infs->eof();
	}
	string getline(const infile& infs)
	{
		std::string str;
		std::getline(*infs,str);
		return str;
	}
	var read(array& args)
	{
		if(args.empty())
			throw syntax_error("Wrong size of arguments.");
		if(args.at(0).type()!=typeid(infile))
			throw syntax_error("Wrong type of arguments.(Request Input File)");
		std::ifstream& infs=*args.at(0).val<infile>(true);
		if(args.size()==1) {
			std::string str;
			infs>>str;
			return parse_value(str);
		}
		else {
			std::string str;
			for(std::size_t i=1; i<args.size(); ++i) {
				infs>>str;
				args.at(i).assign(parse_value(str),true);
			}
		}
		return number(0);
	}
	var write(array& args)
	{
		if(args.empty())
			throw syntax_error("Wrong size of arguments.");
		if(args.at(0).type()!=typeid(outfile))
			throw syntax_error("Wrong type of arguments.(Request Output File)");
		std::ofstream& outfs=*args.at(0).val<outfile>(true);
		for(std::size_t i=1; i<args.size(); ++i)
			outfs<<args.at(i);
		return number(0);
	}
	void init()
	{
		file_ext.add_var("read_method",var::make_constant<file_method>(file_method::read));
		file_ext.add_var("write_method",var::make_constant<file_method>(file_method::write));
		file_ext.add_var("open",var::make_protect<native_interface>(cni(open)));
		file_ext.add_var("is_open",var::make_protect<native_interface>(cni(is_open)));
		file_ext.add_var("eof",var::make_protect<native_interface>(cni(eof)));
		file_ext.add_var("getline",var::make_protect<native_interface>(cni(getline)));
		file_ext.add_var("read",var::make_protect<native_interface>(read));
		file_ext.add_var("write",var::make_protect<native_interface>(write));
		infile_ext.add_var("is_open",var::make_protect<native_interface>(cni(is_open)));
		infile_ext.add_var("eof",var::make_protect<native_interface>(cni(eof)));
		infile_ext.add_var("getline",var::make_protect<native_interface>(cni(getline)));
		infile_ext.add_var("read",var::make_protect<native_interface>(read));
		outfile_ext.add_var("write",var::make_protect<native_interface>(write));
	}
}
