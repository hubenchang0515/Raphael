# Raphael
Raphael is a tiny antivirus software based on [Qt Quick](https://www.qt.io/) and [ClamAV](https://www.clamav.net/)  

You can try to use this software with [Eicar Virus Sample](https://www.eicar.org/?page_id=3950).  

## Note
> ClamAV uses the standard Windows API, which cannot access some files. It's usually because of 2 things, both are normal.
> 1. Another application opened the file without specifying read sharing.
> 2. ACL settings don't allow read access. (If you're using Vista or later, right-click on a file and go to properties, then select the security tab)

![Main](https://github.com/hubenchang0515/resource/blob/master/Raphael/main.png?raw=true)
![Scan](https://github.com/hubenchang0515/resource/blob/master/Raphael/scan.png?raw=true)