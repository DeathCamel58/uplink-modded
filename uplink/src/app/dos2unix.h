#if !defined __DOS2UNIX_H

#include <iostream>
#include <fstream>

using namespace std;

class dos2unixbuf : public streambuf {
public:
#ifdef WIN32
	typedef ios::open_mode openmode;
#else
	typedef ios::openmode openmode;
#endif

  dos2unixbuf(const char *filename, openmode mode);
  ~dos2unixbuf() override;

  void close();

protected:
  int overflow(int c = EOF) override;
  int underflow() override;
  int uflow() override;
  int pbackfail(int c = EOF) override;
  int sync() override;
  
private:
  filebuf inner;
  char *buffer;
};

class idos2unixstream
  : public istream {
public:
  idos2unixstream(const char *filename)
    : istream(new dos2unixbuf(filename, ios::in))
    {
    }

  ~idos2unixstream() override;

  void close();
};

#endif // __DOS2UNIX_H
