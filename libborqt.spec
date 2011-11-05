%define name libborqt
%define version 6.9.0
%define release 1

Summary: This is libborqt as required by Borland Kylix Software
Name: %{name}
Version: %{version}
Release: %{release}
Source0: %{name}-%{version}.tar.bz2
License: GPL
Group: System/Libraries
BuildRoot: %{_tmppath}/%{name}-buildroot
Prefix: %{_prefix}
Packager: Robert Sandilands <rsandila@netscape.net>

%description
This is libborqt as required by Borland Kylix Software. This is as distributed by Borland
with Kylix 3. According to the GPL license I have to provide you with the following URL
that points to the sources: ftp://ftp.trolltech.com/qt/source/qt-x11-2.3.0.tar.gz

%prep
%setup -q

%build

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/lib
cp lib/%{name}-%{version}-qt2.3.so $RPM_BUILD_ROOT/lib/

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/lib/%{name}-%{version}-qt2.3.so


%changelog
* Mon Dec 02 2002 Robert Sandilands <rsandila@netscape.net>
- First release as RPM

# end of file
