#ifndef GENERICSEQ_H_
#define GENERICSEQ_H_

#include <string>
#include <sstream>
#include <iostream>

class GenericSeq {

protected:
	std::string id;
	std::string description;
	size_t length;
	unsigned char* tab;

public:
	GenericSeq();
	GenericSeq(std::string, std::string, unsigned char*);
	GenericSeq(GenericSeq const&);
	GenericSeq &operator=(GenericSeq const&);
	virtual ~GenericSeq();

	char operator[](size_t) const;
	void set_id(std::string);
	std::string get_id() const;
	void set_description(std::string);
	std::string get_description() const;
	size_t get_length() const;
	std::string get_seq() const;
	char get_symbol_at(size_t) const;
	void set_symbol_at(size_t, char);
	GenericSeq get_sub_seq(size_t, size_t) const;
	std::string get_defline() const;
	virtual void print() const;
	GenericSeq revcom() const;
};


std::ostream &operator<<(std::ostream&, const GenericSeq&);


#endif /* GENERICSEQ_H_ */