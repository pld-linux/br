Summary:	Biorhythmus
Summary(pl):	biorytm
Name:		br
Version:	0.1
Release:	1
License:	?
Group:		Applications
Source0:	http://www.sta.to/ftp/%{name}.tar.gz
# Source0-md5:	095229028b82631664116631e5caf93a
Source1:	http://www.sta.to/ftp/%{name}.c
# Source1-md5:	a11826334de33059a0894dcc63880049
Patch0:		%{name}-accept-cmdline.patch
BuildRoot:	%{tmpdir}/%{name}-%{version}-root-%(id -u -n)

%description
Simple biorhythm-graph ascii-art drawer.

%description -l pl
Prosty kre¶larz biorytmów w ascii-art.

%package cgi
Summary:	aqq
Summary(pl):	aqq
Group:		Applications

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

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/*
#%%{_datadir}/%{name}

%files cgi
%defattr(644,root,root,755)
#%doc extras/*.gz
#%%{_datadir}/%{name}-ext
