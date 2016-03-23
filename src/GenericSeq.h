#ifndef GENERICSEQ_H_
#define GENERICSEQ_H_

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>

class GenericSeq {

private:
    std::string id;
    std::string description;
    size_t length;
    unsigned char* tab;

public:
    GenericSeq();
    GenericSeq(const std::string&, const std::string&, const std::string&);
    GenericSeq(size_t);
    GenericSeq(GenericSeq const&);
    GenericSeq& operator=(GenericSeq const&);
    virtual ~GenericSeq();

    char operator[](size_t) const;
    void set_id(const std::string &);
    const std::string& get_id() const;
    void set_description(const std::string &);
    const std::string& get_description() const;
    size_t get_length() const;
    std::string get_seq() const;
    char get_symbol_at(size_t) const;
    void set_symbol_at(size_t, char);
    std::string get_sub_seq(size_t, size_t) const;
    std::string get_defline() const;
    void print() const;
    GenericSeq revcom() const;
    void read(std::ifstream&);
    void write(std::ostream&) const;
};

std::ostream& operator<<(std::ostream&, const GenericSeq&);
std::ifstream& operator>>(std::ifstream&, GenericSeq&);

#endif /* GENERICSEQ_H_ */
