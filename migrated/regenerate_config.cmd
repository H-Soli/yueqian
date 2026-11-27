@echo off
call "E:\TI\sysconfig\sysconfig_cli.bat" -s "E:\TI\mspm0_sdk_2_04_00_06\.metadata\product.json" --device MSPM0G3507 --package "LQFP-64(PM)" -o "%~dp0." "%~dp0yueqian.syscfg"
