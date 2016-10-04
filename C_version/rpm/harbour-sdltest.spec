Name: harbour-rubbybird100
Summary: Rub 'em all !
Version: 1.0
Release: 1
Group: Applications
License: LICENSE
URL: https://gameblabla.nl
Source0: %{name}-%{version}.tar.bz2
BuildRequires: pkgconfig(sdl2)
BuildRequires: pkgconfig(egl)
BuildRequires: pkgconfig(glesv1_cm)
BuildRequires: pkgconfig(glesv2)
#BuildRequires: pkgconfig(openal)

%description
Rub 'em all, Kep 'em alive !

%prep
%setup -q -n %{name}-%{version}
# Command used to configure / set up the application
#%configure

%build
# Command used to compile the application
make -f Makefile.sail

%install
rm -rf %{buildroot}
# Command used to install files into %{buildroot}
make -f Makefile.sail install

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png
%{_datadir}/%{name}/data/alarm.wav
%{_datadir}/%{name}/data/bird.bmp
%{_datadir}/%{name}/data/chop.ogg
%{_datadir}/%{name}/data/ddot.bmp
%{_datadir}/%{name}/data/dies.wav
%{_datadir}/%{name}/data/fond2.bmp
%{_datadir}/%{name}/data/fond.bmp
%{_datadir}/%{name}/data/go.bmp
%{_datadir}/%{name}/data/inst.bmp
%{_datadir}/%{name}/data/left.bmp
%{_datadir}/%{name}/data/limit_score.bmp
%{_datadir}/%{name}/data/logo.bmp
%{_datadir}/%{name}/data/rubby.ogg
%{_datadir}/%{name}/data/score.bmp
%{_datadir}/%{name}/data/tap.bmp
%{_datadir}/%{name}/data/tick.wav
%{_datadir}/%{name}/data/tweet.bmp
%{_datadir}/%{name}/data/copyright.bmp
