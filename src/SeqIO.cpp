#include "SeqIO.h"

using namespace std;

SeqIO::SeqIO(string f, const char d): file(f), direction(d), fd(0)
{
	if( direction == 'r' ) {
		// opening for reading
		fd.open(file.c_str(), ios::in);
	} else if ( direction == 'w') {
		// opening for writing
		fd.open(file.c_str(), ios::out);
	}
}

SeqIO::~SeqIO()
{
	fd.close();
}

GenericSeq SeqIO::next()
{
	enum state {begin, next};
	string line, seq, id, desc;

	while( getline(fd, line, '\n') )
	{
		line.replace('\n', 0, "");

		if( line.at(0) == '>' ) {
			size_t pos = line.find(" ");
			id   = line.substr(1, pos-1);
			desc = line.substr(pos+1, string::npos);
		} else seq += line;
	}

	unsigned char* t = new unsigned char[seq.size()+1];

 	for(size_t i = 0; i < seq.size(); i++) {
 		t[i] = seq[i];
 		//cout << t[i];
 	}

/*
	cout << seq.length() << endl;
	cout << seq << endl;
*/

	GenericSeq gs(id, desc, t);
	delete t; t = NULL;
	return gs;
}

/*
 * Overload >> operator
 */
/*
istream& operator>>(istream& ins, GenericSeq& gs)
{
	string line, id, desc, seq;

	while (getline(ins, line, ' '))
	{
		if(line.at(0) == '>')
		{
			id = line.substr(1,line.size()-1);
			getline(ins, line);
			desc = line;
		}

		else seq += line;
	}

	size_t size = seq.size();
	unsigned char* t = new unsigned char[size];

	for(size_t i = 0; i < size; i++) {
		t[i] = seq[0];
	}

	gs = GenericSeq(id, "", t);

	return ins;
}
*/
