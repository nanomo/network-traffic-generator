%define name traffic
%define version 0.1.3
%define release 1mdk_gui

Summary: This is a traffic generator that can be used to test routers and firewalls under high load conditions.
Name: %{name}
Version: %{version}
Release: %{release}
Source0: %{name}-%{version}.tgz
License: GPL
Group: Development/Tools
BuildRoot: %{_tmppath}/%{name}-buildroot
Prefix: %{_prefix}
URL: http://rsandila.webhop.org/traffic.html
BuildRequires: fastdep

%description
Traffic is a network traffic generator following a server/client model
for generating high volumes of traffic on a network. This could be
used to test the ability of a router/firewall/VPN to handle continuous
high traffic loads. It is optimally used in an development
environment. It does not measure throughput or number of connections
per second or anything like this.

%prep
%setup -q

%build
# alias --with    --define "_with_!#:+       --with-!#:+"
autoconf
./configure --prefix $RPM_BUILD_ROOT/usr/local %{?_with_kylix3}
make

%install
rm -rf $RPM_BUILD_ROOT
make install


%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc AUTHORS CHANGELOG LICENSE KNOWNBUGS
/usr/local/bin/trafserver
/usr/local/bin/trafclient
/usr/local/bin/xtrafserver
/usr/local/bin/xtrafclient
%dir /usr/local/lib/traffic
/usr/local/lib/traffic/client_fixed.so
/usr/local/lib/traffic/payload_random.so   
/usr/local/lib/traffic/reply_ack.so 
/usr/local/lib/traffic/payload_fixed.so        
/usr/local/lib/traffic/protocol_tcp.so     
/usr/local/lib/traffic/reply_echo.so 
/usr/local/lib/traffic/payload_increasing.so   
/usr/local/lib/traffic/protocol_udp.so     
/usr/local/lib/traffic/server_basic.so 
%doc README 
%doc doc/README.client doc/README.client_fixed
%doc doc/README.payload doc/README.payload_fixed doc/README.payload_increasing doc/README.payload_random
%doc doc/README.reply doc/README.reply_ack doc/README.reply_echo
%doc doc/README.interface doc/README.interface_cmdline 
%doc doc/README.server doc/README.server_basic
%doc doc/README.protocols           

%changelog
* Mon Dec 02 2002 Robert Sandilands <rsandila@netscape.net>
- Kylix port was added
- Windows port was improved
- Minor Bugs fixed with linking some of the shared libraries
* Fri Nov 15 2002 Robert Sandilands <rsandila@netscape.net>
- Windows port was added
* Fri Nov 01 2002 Robert Sandilands <rsandila@netscape.net>
- Initial RPM for 0.1.1

# end of file
