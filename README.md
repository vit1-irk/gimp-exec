### gimp-exec

Simple tool for scheme script execution. It's a script-fu client used to shove a source code file to GIMP.

##### Run GIMP Server

You can run server that way from GUI: "Filters" => "Script-Fu" => "Start Server..."

or in a command line mode:

```
gimp -ic -b "(plug-in-script-fu-server 1 \"127.0.0.1\" 10008 \"/tmp/gimp-log\")"
```

##### Send your file

```
$ make
$ ./gimp-exec test.scheme # default ip and port are 127.0.0.1 and 10008
$ ./gimp-exec yourfile.scheme 192.168.1.103 11228
```

##### Limitation

Program removes newline characters when sending file, so you can't use comments in your sources.
