package Win32::Internet;
#######################################################################
#
# Win32::Internet - Perl Module for Internet Extensions
# ^^^^^^^^^^^^^^^
# This module creates an object oriented interface to the Win32
# Internet Functions (WININET.DLL).
#
# Version: 0.06 (26 Jan 1997)
#
#######################################################################

require Exporter;       # to export the constants to the main:: space
require DynaLoader;     # to dynuhlode the module.

use Win32::WinError;    # for windows constants.

@ISA= qw( Exporter DynaLoader );
@EXPORT = qw(
    HTTP_ADDREQ_FLAG_ADD
    HTTP_ADDREQ_FLAG_REPLACE
    HTTP_QUERY_ALLOW
    HTTP_QUERY_CONTENT_DESCRIPTION
    HTTP_QUERY_CONTENT_ID
    HTTP_QUERY_CONTENT_LENGTH
    HTTP_QUERY_CONTENT_TRANSFER_ENCODING
    HTTP_QUERY_CONTENT_TYPE
    HTTP_QUERY_COST
    HTTP_QUERY_CUSTOM
    HTTP_QUERY_DATE
    HTTP_QUERY_DERIVED_FROM
    HTTP_QUERY_EXPIRES
    HTTP_QUERY_FLAG_REQUEST_HEADERS
    HTTP_QUERY_FLAG_SYSTEMTIME
    HTTP_QUERY_LANGUAGE
    HTTP_QUERY_LAST_MODIFIED
    HTTP_QUERY_MESSAGE_ID
    HTTP_QUERY_MIME_VERSION
    HTTP_QUERY_PRAGMA
    HTTP_QUERY_PUBLIC
    HTTP_QUERY_RAW_HEADERS
    HTTP_QUERY_RAW_HEADERS_CRLF
    HTTP_QUERY_REQUEST_METHOD
    HTTP_QUERY_SERVER
    HTTP_QUERY_STATUS_CODE
    HTTP_QUERY_STATUS_TEXT
    HTTP_QUERY_URI
    HTTP_QUERY_USER_AGENT
    HTTP_QUERY_VERSION
    HTTP_QUERY_WWW_LINK
    ICU_BROWSER_MODE
    ICU_DECODE
    ICU_ENCODE_SPACES_ONLY
    ICU_ESCAPE
    ICU_NO_ENCODE
    ICU_NO_META
    ICU_USERNAME
    INTERNET_CONNECT_FLAG_PASSIVE
    INTERNET_FLAG_ASYNC
    INTERNET_HYPERLINK
    INTERNET_FLAG_KEEP_CONNECTION
    INTERNET_FLAG_MAKE_PERSISTENT
    INTERNET_FLAG_NO_AUTH
    INTERNET_FLAG_NO_AUTO_REDIRECT
    INTERNET_FLAG_NO_CACHE_WRITE
    INTERNET_FLAG_NO_COOKIES
    INTERNET_FLAG_READ_PREFETCH
    INTERNET_FLAG_RELOAD
    INTERNET_FLAG_RESYNCHRONIZE
    INTERNET_FLAG_TRANSFER_ASCII
    INTERNET_FLAG_TRANSFER_BINARY
    INTERNET_INVALID_PORT_NUMBER
    INTERNET_INVALID_STATUS_CALLBACK
    INTERNET_OPEN_TYPE_DIRECT
    INTERNET_OPEN_TYPE_PROXY
    INTERNET_OPEN_TYPE_PROXY_PRECONFIG
    INTERNET_OPTION_CONNECT_BACKOFF
    INTERNET_OPTION_CONNECT_RETRIES
    INTERNET_OPTION_CONNECT_TIMEOUT
    INTERNET_OPTION_CONTROL_SEND_TIMEOUT
    INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT
    INTERNET_OPTION_DATA_SEND_TIMEOUT
    INTERNET_OPTION_DATA_RECEIVE_TIMEOUT
    INTERNET_OPTION_HANDLE_SIZE
    INTERNET_OPTION_LISTEN_TIMEOUT
    INTERNET_OPTION_PASSWORD
    INTERNET_OPTION_READ_BUFFER_SIZE
    INTERNET_OPTION_USER_AGENT
    INTERNET_OPTION_USERNAME
    INTERNET_OPTION_VERSION
    INTERNET_OPTION_WRITE_BUFFER_SIZE
    INTERNET_SERVICE_FTP
    INTERNET_SERVICE_GOPHER
    INTERNET_SERVICE_HTTP
    INTERNET_STATUS_CLOSING_CONNECTION
    INTERNET_STATUS_CONNECTED_TO_SERVER    
    INTERNET_STATUS_CONNECTING_TO_SERVER
    INTERNET_STATUS_CONNECTION_CLOSED
    INTERNET_STATUS_HANDLE_CLOSING
    INTERNET_STATUS_HANDLE_CREATED
    INTERNET_STATUS_NAME_RESOLVED
    INTERNET_STATUS_RECEIVING_RESPONSE
    INTERNET_STATUS_REDIRECT    
    INTERNET_STATUS_REQUEST_COMPLETE    
    INTERNET_STATUS_REQUEST_SENT    
    INTERNET_STATUS_RESOLVING_NAME    
    INTERNET_STATUS_RESPONSE_RECEIVED
    INTERNET_STATUS_SENDING_REQUEST    
);


#######################################################################
# This AUTOLOAD is used to 'autoload' constants from the constant()
# XS function.  If a constant is not found then control is passed
# to the AUTOLOAD in AutoLoader.
#

sub AUTOLOAD {
    my($constname);
    ($constname = $AUTOLOAD) =~ s/.*:://;
    #reset $! to zero to reset any current errors.
    $!=0;
    my $val = constant($constname, @_ ? $_[0] : 0);
    if ($! != 0) {

      # [dada] This results in an ugly Autoloader error
      #if ($! =~ /Invalid/) {
      #  $AutoLoader::AUTOLOAD = $AUTOLOAD;
      #  goto &AutoLoader::AUTOLOAD;
      #} else {
    
      # [dada] ... I prefer this one :)

        ($pack,$file,$line) = caller;
        die "Win32::Internet::$constname is not defined, used at $file line $line.";

      #}
    }
    eval "sub $AUTOLOAD { $val }";
    goto &$AUTOLOAD;
}


#######################################################################
# STATIC OBJECT PROPERTIES
#
$VERSION="0.06";

%callback_code=();
%callback_info=();


#######################################################################
# PUBLIC METHODS
#

#======== ### CLASS CONSTRUCTOR
sub new {
#========
  my $class="";
  my $useragent="";
  my $proxy="";
  my $proxybypass="";
  my $flags=0;
  ($class,$useragent,$opentype,$proxy,$proxybypass,$flags)=@_;
  my $self={};  
  my $handle="";

  if(ref($useragent) eq "HASH") {
    $opentype=$useragent->{'opentype'};
    $proxy=$useragent->{'proxy'};
    $proxybypass=$password->{'proxybypass'};
    $flags=$useragent->{'flags'};
    # [dada] $useragent=$useragent->{'useragent'}!!!!
    my $myuseragent=$useragent->{'useragent'};
    undef $useragent;
    $useragent=$myuseragent;
  }

  $useragent="Perl-Win32::Internet/".$Win32::Internet::VERSION if $useragent eq undef;
  $opentype=constant("INTERNET_OPEN_TYPE_DIRECT",0) if $opentype eq undef;

  $handle = InternetOpen($useragent, $opentype, $proxy, $proxybypass, $flags);
  if ($handle) {
    $self->{'connections'} = 0;
    $self->{'pasv'}        = 0;
    $self->{'handle'}      = $handle; 
    $self->{'useragent'}   = $useragent;
    $self->{'proxy'}       = $proxy;
    $self->{'proxybypass'} = $proxybypass;
    $self->{'flags'}       = $flags;
    $self->{'Type'}        = "Internet";
    
    # [dada] I think it's better to call SetStatusCallback explicitly...
    #if($flags & &constant("INTERNET_FLAG_ASYNC",0)) {
    #  my $callbackresult=InternetSetStatusCallback($handle);
    #  if($callbackresult==&constant("INTERNET_INVALID_STATUS_CALLBACK",0)) {
    #    $self->{'Error'}=-2;
    #  }
    #}

    bless $self;
  } else {
    $self->{'handle'} = undef;
    bless $self;
  }
$self;
}  


#============
sub OpenURL {
#============
  my $self="";
  my $new="";
  my $URL="";
  ($self,$new,$URL)=@_;
  return undef if !ref($self);

  my $newhandle="";
  $newhandle=InternetOpenUrl($self->{'handle'},$URL,"",0,0,0);
  if($newhandle eq undef) {
    $self->{'Error'}="Cannot open URL.";
    return undef;
  } else {
    $self->{'connections'}++;
    $_[1]=_new($newhandle);
    $_[1]->{'Type'}="URL";
    $_[1]->{'URL'}=$URL;
    return $newhandle;
  }
}


#================
sub TimeConvert {
#================
  my $self="";
  my $sec=0;
  my $min=0;
  my $hour=0;
  my $day=0;
  my $mon=0;
  my $year=0;
  my $wday=0;
  my $rfc=0;
  ($self,$sec,$min,$hour,$day,$mon,$year,$wday,$rfc)=@_;
  return undef unless ref($self);

  if($rfc eq undef) {
    return InternetTimeToSystemTime($sec);
  } else {
    return InternetTimeFromSystemTime($sec,$min,$hour,$day,$mon,$year,$wday,$rfc);
  }
}


#=======================
sub QueryDataAvailable {
#=======================
  my $self="";
  ($self)=@_;
  return undef if !ref($self);
  
  my $howmuch="";
  $howmuch=InternetQueryDataAvailable($self->{'handle'});
  return $howmuch;
}


#=============
sub ReadFile {
#=============
  my $self="";
  my $buffersize=0;
  ($self,$buffersize)=@_;
  return undef if !ref($self);

  my $content="";
  my $howmuch="";
  $howmuch=InternetQueryDataAvailable($self->{'handle'});
  $buffersize=$howmuch if $buffersize eq undef;
  $content=InternetReadFile($self->{'handle'},($howmuch<$buffersize) ? $howmuch : $buffersize);
  return $content;
}


#===================
sub ReadEntireFile {
#===================
  my $handle="";
  ($handle)=@_;
  my $content="";
  my $buffersize=16000;
  my $howmuch="";
  my $buffer="";

  $handle=$handle->{'handle'} if ref($handle);

  $howmuch=InternetQueryDataAvailable($handle);
  # print "\nReadEntireFile: $howmuch bytes to read...\n";
  while($howmuch>0) {
    $buffer=InternetReadFile($handle,($howmuch<$buffersize) ? $howmuch : $buffersize);
    # print "\nReadEntireFile: ",length($buffer)," bytes read...\n";
    if($buffer eq undef) {
      return undef;
    } else {
      $content.=$buffer;
    }
    $howmuch=InternetQueryDataAvailable($handle);
    # print "\nReadEntireFile: still $howmuch bytes to read...\n";
  }
  return $content;
}


#=============
sub FetchURL {
#=============
  # (OpenURL+Read+Close)...
  my $self="";
  my $URL="";
  ($self,$URL)=@_;
  return undef if !ref($self);

  my $newhandle="";
  my $content="";

  $newhandle=InternetOpenUrl($self->{'handle'},$URL,"",0,0,0);
  $self->{'Error'}="Cannot open URL." if $newhandle eq undef;
  return undef if $newhandle eq undef;
  $content=ReadEntireFile($newhandle);
  InternetCloseHandle($newhandle);
  return $content;
}


#================
sub Connections {
#================
  my $self="";
  ($self)=@_;
  return undef if !ref($self);

  return $self->{'connections'} if $self->{'Type'} eq "Internet";
  return undef;
}


#================
sub GetResponse {
#================
  my $num=0;
  my $text="";
  ($num,$text)=InternetGetLastResponseInfo();
  return $text;
}


#==============
sub UserAgent {
#==============
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);

  my $retval="";
  my $option=constant("INTERNET_OPTION_USER_AGENT",0);

  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#=============
sub Username {
#=============
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);
  if($self->{'Type'} ne "HTTP" && $self->{'Type'} ne "FTP") {
    $self->{'Error'}="Username() only on FTP or HTTP sessions.";
    return undef;
  }

  my $retval="";
  my $option=constant("INTERNET_OPTION_USERNAME",0);
  
  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#=============
sub Password {
#=============
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);

  if($self->{'Type'} ne "HTTP" && $self->{'Type'} ne "FTP") {
    $self->{'Error'}="Password() only on FTP or HTTP sessions.";
    return undef;
  }

  my $retval="";
  my $option=constant("INTERNET_OPTION_PASSWORD",0);

  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#===================
sub ConnectTimeout {
#===================
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);

  my $retval="";
  my $option=constant("INTERNET_OPTION_CONNECT_TIMEOUT",0);

  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#===================
sub ConnectRetries {
#===================
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);

  my $retval="";
  my $option=constant("INTERNET_OPTION_CONNECT_RETRIES",0);

  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#===================
sub ConnectBackoff {
#===================
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);

  my $retval="";
  my $option=constant("INTERNET_OPTION_CONNECT_BACKOFF",0);

  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#====================
sub DataSendTimeout {
#====================
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);

  my $retval="";
  my $option=constant("INTERNET_OPTION_DATA_SEND_TIMEOUT",0);

  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#=======================
sub DataReceiveTimeout {
#=======================
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);

  my $retval="";
  my $option=constant("INTERNET_OPTION_DATA_RECEIVE_TIMEOUT",0);

  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#==========================
sub ControlReceiveTimeout {
#==========================
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);

  my $retval="";
  my $option=constant("INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT",0);

  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#=======================
sub ControlSendTimeout {
#=======================
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef if !ref($self);

  my $retval="";
  my $option=constant("INTERNET_OPTION_CONTROL_SEND_TIMEOUT",0);

  if($value eq undef) {
    $retval=InternetQueryOption($self->{'handle'},$option);
  } else {
    $retval=InternetSetOption($self->{'handle'},$option,$value);
  }
  return $retval;
}


#================
sub QueryOption {
#================
  my $self="";
  my $value="";
  ($self,$option)=@_;
  return undef if !ref($self);

  my $retval="";
  $retval=InternetQueryOption($self->{'handle'},$option);
  return $retval;
}


#==============
sub SetOption {
#==============
  my $self="";
  my $option=0;
  my $value="";
  ($self,$option,$value)=@_;
  return undef if !ref($self);

  my $retval="";
  $retval=InternetSetOption($self->{'handle'},$option,$value);
  return $retval;
}


#=============
sub CrackURL {
#=============
  my $self="";
  my $URL="";
  my $flags=0;
  ($self,$URL,$flags)=@_;
  return undef if !ref($self);
  my @newurl=();
  $flags=constant("ICU_ESCAPE",0) if $flags eq undef;
  @newurl=InternetCrackUrl($URL,$flags);
  if(@newurl[0] eq undef) {
    $self->{'Error'}="Cannot crack URL.";
    return undef;
  } else {
    return @newurl;
  }
}


#==============
sub CreateURL {
#==============
  my ($self,$scheme,$hostname,$port,$username,$password,$path,$extrainfo,$flags)=@_;
  return undef if !ref($self);
  if(ref($scheme) eq "HASH") {
    $flags=$hostname;
    $hostname=$scheme->{'hostname'};
    $port=$scheme->{'port'};
    $username=$scheme->{'username'};
    $password=$scheme->{'password'};
    $path=$scheme->{'path'};
    $extrainfo=$scheme->{'extrainfo'};
    my $myscheme=$scheme->{'scheme'};
    undef $scheme;
    $scheme=$myscheme;
  }
  my $newurl="";
  $flags=constant("ICU_ESCAPE",0) if $flags eq undef;
  $newurl=InternetCreateUrl($scheme,$hostname,$port,
                            $username,$password,
                            $path,$extrainfo,$flags);
  if($newurl eq undef) {
    $self->{'Error'}="Cannot create URL.";
    return undef;
  } else {
    return $newurl;
  }
}


#====================
sub CanonicalizeURL {
#====================
  my ($self,$URL,$flags)=@_;
  return undef if !ref($self);
  my $newurl="";
  $newurl=InternetCanonicalizeUrl($URL,$flags);
  if($newurl eq undef) {
    $self->{'Error'}="Cannot canonicalize URL.";
    return undef;
  } else {
    return $newurl;
  }
}


#===============
sub CombineURL {
#===============
  my ($self,$baseURL,$relativeURL,$flags)=@_;
  return undef if !ref($self);
  my $newurl="";
  $newurl=InternetCombineUrl($baseURL,$relativeURL,$flags);
  if($newurl eq undef) {
    $self->{'Error'}="Cannot combine URL(s).";
    return undef;
  } else {
    return $newurl;
  }
}


#======================
sub SetStatusCallback {
#======================
  my $self="";
  my $context=0;
  ($self,$context)=@_;
  return undef if !ref($self);
  
  my $callback=InternetSetStatusCallback($self->{'handle'});
  if($callback==constant("INTERNET_INVALID_STATUS_CALLBACK",0)) {
    return undef;
  } else {
    return $callback;  
  }
}


#======================
sub GetStatusCallback {
#======================
  my $self="";
  my $context=0;
  ($self,$context)=@_;
  $context=$self if not defined $context;
  return($callback_code{$context}, $callback_info{$context});
}


#==========
sub Error {
#==========
  my $self="";
  ($self)=@_;
  return undef if !ref($self);
  
  my $errnum=0;
  my $errtext="";
  my $tmp="";
  
  $errnum=Win32::GetLastError();
  if($errnum<12000) {
    $errtext=Win32::FormatMessage($errnum);
    $errtext=~s/[\r\n]//g;
  } elsif($errnum==12006) {
    $errtext="The URL scheme could not be recognized, or is not supported.";
  } elsif($errnum==12005) {
    $errtext="The URL is invalid.";
  } elsif($errnum==12007) {
    $errtext="The server name could not be resolved.";
  } elsif($errnum==12009) {
    $errtext="A query to InternetQueryOption or InternetSetOption specified an invalid option value.";
  } elsif($errnum==12010) {
    $errtext="The length of an option supplied to InternetQueryOption or InternetSetOption is incorrect for the type of option specified.";
  } elsif($errnum==12019) {
    $errtext="The requested operation cannot be carried out because the handle supplied is not in the correct state.";
  } elsif($errnum==12029) {
    $errtext="The attempt to connect to the server failed.";
  } elsif($errnum==12031) {
    $errtext="The connection with the server has been reset.";
  } elsif($errnum==12152) {
    $errtext="The server response could not be parsed.";
  } elsif($errnum==12111) {
    $errtext="The FTP operation was not completed because the session was aborted.";
  } elsif($errnum==12003) {
    ($tmp,$errtext)=InternetGetLastResponseInfo();
    chomp $errtext;
    1 while($errtext=~s/(.*)\n//); # the last line should be significative... 
                                   # otherwise call GetResponse() to get it whole
  } else {
    $errtext="Error";
  }
  if($errnum==0 && $self->{'Error'} ne undef) { 
    if($self->{'Error'}==-2) {
      $errnum=-2;
      $errtext="Asynchronous operations not available.";
    } else {
      $errnum=-1;
      $errtext=$self->{'Error'};
    }
  }
  return (wantarray)? ($errnum, $errtext) : "\[".$errnum."\] ".$errtext;
}


#============
sub Version {
#============
  my $dll = InternetDllVersion();
  $dll=~s/\0//g;
  return (wantarray)? ($Win32::Internet::Version, $dll) 
                    : $Win32::Internet::Version."/".$dll;
}


#==========
sub Close {
#==========
  my $self="";
  my $handle="";
  ($self,$handle)=@_;
  if($handle eq undef) {
    return undef if !ref($self);
    $handle=$self->{'handle'};
  }
  InternetCloseHandle($handle);
}



#######################################################################
# FTP CLASS METHODS
#

#======== ### FTP CONSTRUCTOR
sub FTP {
#========
  my $self="";
  my $new="";
  my $server="";
  my $username="";
  my $password="";
  my $port=0;
  my $pasv=0;
  my $context=0;
  ($self,$new,$server,$username,$password,$port,$pasv,$context)=@_;    
  return undef if !ref($self);

  my $retval='';
  my $newhandle='';
  my $pasv='';

  if(ref($server) eq "HASH") {
    $port=$server->{'port'};
    $username=$server->{'username'};
    $password=$password->{'host'};
    my $myserver=$server->{'server'};
    $pasv=$server->{'pasv'};
    $context=$server->{'context'};
    undef $server;
    $server=$myserver;
  }

  $pasv=$self->{'pasv'} if $pasv eq undef;
  $pasv=constant("INTERNET_CONNECT_FLAG_PASSIVE",0) if $pasv ne 0;
  $port=21 if !$port;
  $context=0 if $context eq undef;

  my $ftp=constant("INTERNET_SERVICE_FTP",0);
  
  # print "FTP.handle=$self->{'handle'}\n";
  # print "FTP.server=$server\nFTP.port=$port\nFTP.username=$username\n";  
  # print "FTP.password=$password\nFTP.ftp=$ftp\nFTP.pasv=$pasv\nFTP.context=$context\n";
  $newhandle=InternetConnect($self->{'handle'},$server,$port,
                             $username,$password,
                             $ftp,$pasv,$context);
  if($newhandle ne undef) {
    $self->{'connections'}++;
    $_[1]=_new($newhandle);
    $_[1]->{'Type'}="FTP";
    $_[1]->{'Mode'}="bin";
    $_[1]->{'pasv'}=$pasv;
    $_[1]->{'username'}=$username;
    $_[1]->{'password'}=$password;
    $_[1]->{'server'}=$server;
    return $newhandle;
  } else {
    return undef;
  }
}

#========
sub Pwd {
#========
  my $self="";
  ($self)=@_;
  return undef if !ref($self);

  my $path="";
  if($self->{'Type'} ne "FTP" || $self->{'handle'} eq undef) {
    $self->{'Error'}="Pwd() only on FTP sessions.";
    return undef;
  }
  $path=FtpGetCurrentDirectory($self->{'handle'});
  return $path;
}


#=======
sub Cd {
#=======
  my $self="";
  my $path="";
  ($self,$path)=@_;
  return undef if !ref($self);

  my $retval="";
  if($self->{'Type'} ne "FTP" || $self->{'handle'} eq undef) {
    $self->{'Error'}="Cd() only on FTP sessions.";
    return undef;
  }
  $retval=FtpSetCurrentDirectory($self->{'handle'},$path);
  if($retval eq undef) {
    return undef;
  } else {
    return $path;
  }
}
#====================
sub Cwd   { Cd(@_); }
sub Chdir { Cd(@_); }
#====================


#==========
sub Mkdir {
#==========
  my $self="";
  my $path="";
  ($self,$path)=@_;
  return undef if !ref($self);

  my $retval="";
  if($self->{'Type'} ne "FTP" || $self->{'handle'} eq undef) {
    $self->{'Error'}="Mkdir() only on FTP sessions.";
    return undef;
  }
  $retval=FtpCreateDirectory($self->{'handle'},$path);
  $self->{'Error'}="Can't create directory." if $retval eq undef;
  return $retval;
}
#====================
sub Md { Mkdir(@_); }
#====================


#=========
sub Mode {
#=========
  my $self="";
  my $value="";
  ($self,$value)=@_;
  return undef unless ref($self);

  if($self->{'Type'} ne "FTP" || $self->{'handle'} eq undef) {
    $self->{'Error'}="Mode() only on FTP sessions.";
    return undef;
  }
  if($value eq undef) {
    return $self->{'Mode'};
  } else {
    my $mode=($value=~/^asc/i)? "Ascii" : "Binary";
    $self->$mode($_[0]);
    # $self->{'Mode'}="asc" if $value=~/^asc/i;
    # $self->{'Mode'}="bin" if $value=~/^bin/i;
  }
  return $self->{'Mode'};
}


#==========
sub Rmdir {
#==========
  my $self="";
  my $path="";
  ($self,$path)=@_;
  return undef if !ref($self);

  my $retval="";
  if($self->{'Type'} ne "FTP" || $self->{'handle'} eq undef) {
    $self->{'Error'}="Rmdir() only on FTP sessions.";
    return undef;
  }
  $retval=FtpRemoveDirectory($self->{'handle'},$path);
  $self->{'Error'}="Can't remove directory." if $retval eq undef;
  return $retval;
}
#====================
sub Rd { Rmdir(@_); }
#====================


#=========
sub Pasv {
#=========
  my $self="";
  my $value=0;
  ($self,$value)=@_;
  return undef if !ref($self);

  if($value ne undef && $self->{'Type'} eq "Internet") {
    if($value==0) {
      $self->{'pasv'}=0;
    } else {
      $self->{'pasv'}=1;
    }
  }
  return $self->{'pasv'};
}


#=========
sub List {
#=========
  my $self="";
  my $pattern="";
  my $retmode=0;
  ($self,$pattern,$retmode)=@_;
  return undef if !ref($self);

  my $retval="";
  my $filename=""; my $altname=""; my $size=""; my $attr=""; my $ctime=""; my $atime=""; my $mtime="";
  my $ctimesec=""; my $ctimemin=""; my $ctimehou=""; my $ctimeday=""; my $ctimemon=""; my $ctimeyea="";
  my $atimesec=""; my $atimemin=""; my $atimehou=""; my $atimeday=""; my $atimemon=""; my $atimeyea="";
  my $mtimesec=""; my $mtimemin=""; my $mtimehou=""; my $mtimeday=""; my $mtimemon=""; my $mtimeyea="";
  my $newhandle="";
  my $nextfile="";
  my @results=();
  my $file;
  if($self->{'Type'} ne "FTP") {
    $self->{'Error'}="List() only on FTP sessions.";
    return undef;
  }
  if($retmode==2) {
    ( $newhandle,$filename,$altname,
      $size,$attr,         
      $ctimesec,$ctimemin,$ctimehou,$ctimeday,$ctimemon,$ctimeyea,
      $atimesec,$atimemin,$atimehou,$atimeday,$atimemon,$atimeyea,
      $mtimesec,$mtimemin,$mtimehou,$mtimeday,$mtimemon,$mtimeyea
    )=FtpFindFirstFile($self->{'handle'},$pattern,0,0);
    if($newhandle eq undef) {
       $self->{'Error'}="Can't read FTP directory.";
       return undef;
     } else {
       $nextfile=1;
       while($nextfile ne undef) {
         $ctime=join(",",($ctimesec,$ctimemin,$ctimehou,$ctimeday,$ctimemon,$ctimeyea));
         $atime=join(",",($atimesec,$atimemin,$atimehou,$atimeday,$atimemon,$atimeyea));
         $mtime=join(",",($mtimesec,$mtimemin,$mtimehou,$mtimeday,$mtimemon,$mtimeyea));
         push(@results,$filename,$altname,$size,$attr,$ctime,$atime,$mtime);
         ( $nextfile,$filename,$altname,
           $size,$attr,
           $ctimesec,$ctimemin,$ctimehou,$ctimeday,$ctimemon,$ctimeyea,
           $atimesec,$atimemin,$atimehou,$atimeday,$atimemon,$atimeyea,
           $mtimesec,$mtimemin,$mtimehou,$mtimeday,$mtimemon,$mtimeyea
         )=InternetFindNextFile($newhandle);      
       }
       InternetCloseHandle($newhandle);
       return @results;
    }
  } elsif($retmode==3) {
    ( $newhandle,$filename,$altname,
      $size,$attr,
      $ctimesec,$ctimemin,$ctimehou,$ctimeday,$ctimemon,$ctimeyea,
      $atimesec,$atimemin,$atimehou,$atimeday,$atimemon,$atimeyea,
      $mtimesec,$mtimemin,$mtimehou,$mtimeday,$mtimemon,$mtimeyea
    )=FtpFindFirstFile($self->{'handle'},$pattern,0,0);
    if($newhandle eq undef) {
       $self->{'Error'}="Can't read FTP directory.";
       return undef;
     } else {
       $nextfile=1;
       while($nextfile ne undef) {
         $ctime=join(",",($ctimesec,$ctimemin,$ctimehou,$ctimeday,$ctimemon,$ctimeyea));
         $atime=join(",",($atimesec,$atimemin,$atimehou,$atimeday,$atimemon,$atimeyea));
         $mtime=join(",",($mtimesec,$mtimemin,$mtimehou,$mtimeday,$mtimemon,$mtimeyea));
         $file={ "name"     => $filename,
                 "altname"  => $altname,
                 "size"     => $size,
                 "attr"     => $attr,
                 "ctime"    => $ctime,
                 "atime"    => $atime,
                 "mtime"    => $mtime,
         };
         push(@results,$file);
         ( $nextfile,$filename,$altname,
           $size,$attr,
           $ctimesec,$ctimemin,$ctimehou,$ctimeday,$ctimemon,$ctimeyea,
           $atimesec,$atimemin,$atimehou,$atimeday,$atimemon,$atimeyea,
           $mtimesec,$mtimemin,$mtimehou,$mtimeday,$mtimemon,$mtimeyea
         )=InternetFindNextFile($newhandle);      
       }
       InternetCloseHandle($newhandle);
       return @results;
    }
  } else {
    ($newhandle,$filename)=FtpFindFirstFile($self->{'handle'},$pattern,0,0);
    # print "List.filename=$test [$size]\n";
    if($newhandle eq undef) {
      $self->{'Error'}="Can't read FTP directory.";
      return undef;
    } else {
      $nextfile=1;
      while($nextfile ne undef) {
        push(@results,$filename);
        ($nextfile,$filename)=InternetFindNextFile($newhandle);  
        # print "List.no more files\n" if $nextfile eq undef;
      }
      InternetCloseHandle($newhandle);
      return @results;
    }
  }
}
#====================
sub Ls  { List(@_); }
sub Dir { List(@_); }
#====================


#=================
sub FileAttrInfo {
#=================
  my $self="";
  my $attr=0;
  ($self,$attr)=@_;
  my $retval="";
  my @attrinfo=();
  push(@attrinfo,"READONLY")   if $attr & 1;
  push(@attrinfo,"HIDDEN")     if $attr & 2;
  push(@attrinfo,"SYSTEM")     if $attr & 4;
  push(@attrinfo,"DIRECTORY")  if $attr & 16;
  push(@attrinfo,"ARCHIVE")    if $attr & 32;
  push(@attrinfo,"NORMAL")     if $attr & 128;
  push(@attrinfo,"TEMPORARY")  if $attr & 256;
  push(@attrinfo,"COMPRESSED") if $attr & 2048;
  return (wantarray)? @attrinfo : join(" ",@attrinfo);
}


#===========
sub Binary {
#===========
  my $self="";
  ($self)=@_;
  return undef if !ref($self);

  if($self->{'Type'} ne "FTP") {
    $self->{'Error'}="Binary() only on FTP sessions.";
    return undef;
  }
  $self->{'Mode'}="bin";
  return undef;
}
#======================
sub Bin { Binary(@_); }
#======================


#==========
sub Ascii {
#==========
  my $self="";
  ($self)=@_;
  return undef if !ref($self);

  if($self->{'Type'} ne "FTP") {
    $self->{'Error'}="Ascii() only on FTP sessions.";
    return undef;
  }
  $self->{'Mode'}="asc";
  return undef;
}
#=====================
sub Asc { Ascii(@_); }
#=====================


#========
sub Get {
#========
  my $self="";
  my $remote="";
  my $local="";
  my $overwrite=0;
  my $flags=0;
  my $context=0;
  ($self,$remote,$local,$overwrite,$flags,$context)=@_;
  return undef if !ref($self);

  my $retval="";
  if($self->{'Type'} ne "FTP") {
    $self->{'Error'}="Get() only on FTP sessions.";
    return undef;
  }
  my $mode=($self->{'Mode'} eq "asc" ? 1 : 2);
  $local=$remote if $local eq undef;
  $flags=0 if $flags eq undef;
  $context=0 if $context eq undef;
  $retval=FtpGetFile($self->{'handle'},$remote,$local,$overwrite,$flags,$mode,$context);
  $self->{'Error'}="Can't get file." if $retval eq undef;
  return $retval;
}


#===========
sub Rename {
#===========
  my $self="";
  my $oldname="";
  my $newname="";
  ($self,$oldname,$newname)=@_;
  return undef if !ref($self);

  my $retval="";
  if($self->{'Type'} ne "FTP") {
    $self->{'Error'}="Rename() only on FTP sessions.";
    return undef;
  }
  $retval=FtpRenameFile($self->{'handle'},$oldname,$newname);
  $self->{'Error'}="Can't rename file." if $retval eq undef;
  return $retval;
}
#======================
sub Ren { Rename(@_); }
#======================


#===========
sub Delete {
#===========
  my $self="";
  my $filename="";
  ($self,$filename)=@_;
  return undef if !ref($self);

  my $retval="";
  if($self->{'Type'} ne "FTP") {
    $self->{'Error'}="Delete() only on FTP sessions.";
    return undef;
  }
  $retval=FtpDeleteFile($self->{'handle'},$filename);
  $self->{'Error'}="Can't delete file." if $retval eq undef;
  return $retval;
}
#======================
sub Del { Delete(@_); }
#======================


#========
sub Put {
#========
  my $self="";
  my $local="";
  my $remote="";
  my $context=0;
  ($self,$local,$remote,$context)=@_;
  return undef if !ref($self);

  my $retval="";
  if($self->{'Type'} ne "FTP") {
    $self->{'Error'}="Put() only on FTP sessions.";
    return undef;
  }
  my $mode=($self->{'Mode'} eq "asc" ? 1 : 2);
  $context=0 if $context eq undef;
  $retval=FtpPutFile($self->{'handle'},$local,$remote,$mode,$context);
  $self->{'Error'}="Can't put file." if $retval eq undef;
  return $retval;
}


#######################################################################
# HTTP CLASS METHODS
#

#========= ### HTTP CONSTRUCTOR
sub HTTP {
#=========
  my $self="";
  my $new="";
  my $server="";
  my $username="";
  my $password="";
  my $port=0;
  my $flags=0;
  my $context=0;
  ($self,$new,$server,$username,$password,$port,$flags,$context)=@_;    
  return undef if !ref($self);

  my $retval='';
  my $newhandle='';

  if(ref($server) eq "HASH") {
    $port=$server->{'port'};
    $username=$server->{'username'};
    $password=$password->{'host'};
    my $myserver=$server->{'server'};
    $flags=$server->{'flags'};
    $context=$server->{'context'};
    undef $server;
    $server=$myserver;
  }
  $port=80 if $port eq undef;
  my $http=constant("INTERNET_SERVICE_HTTP",0);
  $flags=0 if $flags eq undef;
  $context=0 if $context eq undef;
  $newhandle=InternetConnect($self->{'handle'},$server,$port,
                             $username,$password,
                             $http,$flags,$context);
  if($newhandle ne undef) {
    $self->{'connections'}++;
    $_[1]=_new($newhandle);
    $_[1]->{'Type'}="HTTP";
    $_[1]->{'username'}=$username;
    $_[1]->{'password'}=$password;
    $_[1]->{'server'}=$server;
    $_[1]->{'accept'}="text/*\0image/gif\0image/jpeg";
    return $newhandle;
  } else {
    return undef;
  }
}


#================
sub OpenRequest {
#================
  # alternatively to Request:
  # it creates a new HTTP_Request object
  # you can act upon it with AddHeader, SendRequest, ReadFile, QueryInfo, Close, ...
  my $self="";
  my $new="";
  my $path="";
  my $method="";
  my $version="";
  my $accept="";
  my $flags=0;
  my $context=0;
  ($self,$new,$path,$method,$version,$referer,$accept,$flags,$context)=@_;
  return undef if !ref($self);

  if($self->{'Type'} ne "HTTP") {
    $self->{'Error'}="SetHeader() only on HTTP sessions.";
    return undef;
  }

  if(ref($path) eq "HASH") {
    $method=$path->{'method'};
    $version=$path->{'version'};
    $referer=$path->{'referer'};
    $accept=$path->{'accept'};
    $flags=$path->{'flags'};
    $context=$path->{'context'};
    my $mypath=$path->{'path'};
    undef $path;
    $path=$mypath;
  }

  my $newhandle="";
  $method="GET" if $method eq undef;
  $path="/" if $path eq undef;
  $path="/".$path if substr($path,0,1) ne "/";
  $version="HTTP/1.0" if $version eq undef; 
  $accept=$self->{'accept'} if $accept eq undef;
  $flags=0 if $flags eq undef;
  $context=0 if $context eq undef;
  $newhandle=HttpOpenRequest($self->{'handle'},
                             $method,$path,$version,$referer,$accept,
                             $flags,$context);
  if($newhandle ne undef) {
    $_[1]=_new($newhandle);
    $_[1]->{'Type'}="HTTP_Request";
    $_[1]->{'method'}=$method;
    $_[1]->{'request'}=$path;
    $_[1]->{'accept'}=$accept;
    return $newhandle;
  } else {
    return undef;
  }
}

#================
sub SendRequest {
#================
  my $self="";
  my $postdata="";
  ($self,$postdata)=@_;
  return undef if !ref($self);

  if($self->{'Type'} ne "HTTP_Request") {
    $self->{'Error'}="AddHeader() only on HTTP requests.";
    return undef;
  }
  $result=HttpSendRequest($self->{'handle'},undef,0,$postdata,length($postdata));
  return $result;
}


#==============
sub AddHeader {
#==============
  my $self="";
  my $header="";
  my $flags=0;
  my($self,$header,$flags)=@_;
  return undef if !ref($self);
  
  if($self->{'Type'} ne "HTTP_Request") {
    $self->{'Error'}="AddHeader() only on HTTP requests.";
    return undef;
  }
  $flags=constant("HTTP_ADDREQ_FLAG_ADD",0) if ($flags eq undef || $flags==0);
  $result=HttpAddRequestHeaders($self->{'handle'},$header,$flags);
  return $result;
}


#==============
sub QueryInfo {
#==============
  my $self="";
  my $header="";
  my $flags=0;
  ($self,$header,$flags)=@_;
  return undef if !ref($self);

  my @queryresult=();
  if($self->{'Type'} ne "HTTP_Request") {
    $self->{'Error'}="QueryInfo() only on HTTP requests.";
    return undef;
  }
  $flags=constant("HTTP_QUERY_CUSTOM",0) if ($flags eq undef && $header ne undef);
  @queryresult=HttpQueryInfo($self->{'handle'},$flags,$header);
  return (wantarray)? @queryresult : join(" ",@queryresult);
}


#============
sub Request {
#============
  # HttpOpenRequest+HttpAddHeaders+HttpSendRequest+InternetReadFile+HttpQueryInfo
  # returns statuscode + headers + body(file)
  my $self="";
  my $path="";
  my $method="";
  my $version="";
  my $referer="";
  my $accept="";
  my $flags=0;
  my $postdata="";
  ($self,$path,$method,$version,$referer,$accept,$flags,$postdata)=@_;
  return undef if !ref($self);

  if($self->{'Type'} ne "HTTP") {
    $self->{'Error'}="Request() only on HTTP sessions.";
    return undef;
  }

  if(ref($path) eq "HASH") {
    $method=$path->{'method'};
    $version=$path->{'version'};
    $referer=$path->{'referer'};
    $accept=$path->{'accept'};
    $flags=$path->{'flags'};
    $postdata=$path->{'postdata'};
    my $mypath=$path->{'path'};
    undef $path;
    $path=$mypath;
  }

  my $newhandle="";
  my $content="";
  my $result="";
  my @queryresult=();
  my $statuscode="";
  my $flags="";
  my $headers="";
  $method="GET" if $method eq undef;
  $path="/" if $path eq undef;
  $path="/".$path if substr($path,0,1) ne "/";
  $version="HTTP/1.0" if $version eq undef; 
  $accept=$self->{'accept'} if $accept eq undef;
  $flags=0 if $flags eq undef;
  # print "\nRequest: calling HttpOpenRequest...\n";
  $newhandle=HttpOpenRequest($self->{'handle'},$method,$path,$version,$referer,$accept,0,$flags);

  if($newhandle ne undef) {
    # print "\nRequest: calling HttpSendRequest...\n";
    $result=HttpSendRequest($newhandle,undef,0,$postdata,length($postdata));

    if($result ne undef) {
      $flags=constant("HTTP_QUERY_STATUS_CODE",0);
      $statuscode=HttpQueryInfo($newhandle,$flags,"");

      $content=ReadEntireFile($newhandle);

      $flags=constant("HTTP_QUERY_RAW_HEADERS_CRLF",0);
      $headers=HttpQueryInfo($newhandle,$flags,"");

      InternetCloseHandle($newhandle);
      return($statuscode, $headers, $content);
    } else {
      return undef;
    }
  } else {
    return undef;
  }
}


#######################################################################
# END OF THE PUBLIC METHODS
#


#========= ### SUB-CLASSES CONSTRUCTOR
sub _new {
#=========
  my $self={};
  if ($_[0]) {
    $self->{'handle'} = $_[0];
    bless $self
  } else {
    undef($self);
  }
  $self;
}


#============ ### CLASS DESTRUCTOR
sub DESTROY {
#============
  my($self)=@_;
  # print "Closing handle $self->{'handle'}...\n";
  InternetCloseHandle($self->{'handle'});
  # ok, now you can die... 
}


#=============
sub callback {
#=============
  #my($self,$name,$status,$info)=@_;

  my $name="";
  my $status=0;
  my $info=0;
  ($name,$status,$info)=@_;
  $callback_code{$name}=$status;
  $callback_info{$name}=$info;
}

#######################################################################
# dynamically load in the Internet.pll module.
#

bootstrap Win32::Internet;

# Preloaded methods go here.

#Currently Autoloading is not implemented in Perl for win32
# Autoload methods go after __END__, and are processed by the autosplit program.

1;
__END__

