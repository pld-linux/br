Summary:	biorytm
Summary(pl):	bri
Name:		br
Version:   0.1
Release:  1
License:   Unknown (enter GPL/LGPL/BSD/BSD-like/other license name here)
Group:		amusment
#Vendor:		-
#Icon:		-
Source0:	http://www.sta.to/ftp/%{name}.tar.gz
Source1:	http://www.sta.to/ftp/%{name}.c
Patch0:		%{name}-accept-cmdline.patch
URL:		-
BuildRoot:	%{tmpdir}/%{name}-%{version}-root-%(id -u -n)

%description
Simple biorytm-graph ascii-art drawer.

%description -l pl
Prosty kre¶larz biorytmów w ascii-art.

%package cgi
Summary:   aqq
Summary(pl):   aqq
Group:		-

%description cgi
cgi

%description cgi -l pl
cgi

%prep
%setup -q -n %{name} 
%patch0 -p0

%build
rm -f %{name}.c
cp %{SOURCE1} ./
%{__cc} -lm -o %{name} %{name}.c

%install
rm -rf $RPM_BUILD_ROOT
install -d $RPM_BUILD_ROOT/usr/bin

install %{name} $RPM_BUILD_ROOT/usr/bin

%clean

rm -rf $RPM_BUILD_ROOT

%pre

%post

%preun

%postun

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/*
#%{_datadir}/%{name}

%files cgi
%defattr(644,root,root,755)
#%doc extras/*.gz
#%{_datadir}/%{name}-ext
