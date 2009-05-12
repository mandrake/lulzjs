/****************************************************************************
* This file is part of lulzJS                                               *
* Copyleft meh. [http://meh.doesntexist.org | meh.ffff@gmail.com]           *
*                                                                           *
* lulzJS is free software: you can redistribute it and/or modify            *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzJS is distributed in the hope that it will be usefull                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzJS.  If not, see <http://www.gnu.org/licenses/>.           *
****************************************************************************/

#ifndef _SYSTEM_NETWORK_SOCKETS_COMMON
#define _SYSTEM_NETWORK_SOCKETS_COMMON

char*
__Sockets_getHostByName (const char* host)
{
    char* buf = new char[PR_NETDB_BUF_SIZE];
    PRHostEnt hp; 
    if (PR_GetHostByName(host, buf, PR_NETDB_BUF_SIZE, &hp) == PR_FAILURE) {
        return NULL;
    }

    char* ip = new char[INET6_ADDRSTRLEN];
    int offset = 0;

    offset += sprintf(&ip[offset], "%u.", (unsigned char) hp.h_addr_list[0][0]);
    offset += sprintf(&ip[offset], "%u.", (unsigned char) hp.h_addr_list[0][1]);
    offset += sprintf(&ip[offset], "%u.", (unsigned char) hp.h_addr_list[0][2]);
    offset += sprintf(&ip[offset], "%u",  (unsigned char) hp.h_addr_list[0][3]);

    delete [] buf;

    return ip;
}

JSBool
__Sockets_isIPv4 (const char* host)
{
    char klass[4];
    memset(klass, 0, 4);
 
    short number;
    size_t i;
    for (i = 0, number = 0; i < strlen(host); i++) {
        if (number == 3) {
            return JS_FALSE;
        }
 
        if (host[i] == '.') {
            number = 0;
            short part = atoi(klass);
 
            if (part < 0 || part > 255) {
                return JS_FALSE;
            }
 
            memset(klass, 0, 4);
        }
 
        klass[number] = host[i];
        number++;
    }
 
    return JS_TRUE;
}

PRStatus
__Sockets_initAddr (PRNetAddr* addr, const char* host, int port = -1)
{
    std::string sHost = host;
 
    if (PR_StringToNetAddr(sHost.c_str(), addr) == PR_FAILURE) {
        std::string sIp;
        std::string sPort;
 
        if (sHost[0] == '[') {
            sIp = sHost.substr(1, sHost.find_last_of("]")-1);
            sPort = sHost.substr(sHost.find_last_of("]")+2);
        }
        else {
            sIp = sHost.substr(0, sHost.find_last_of(":"));
            sPort = sHost.substr(sHost.find_last_of(":")+1);
        }

        if (!sPort.empty()) {
            int tmp = atoi(sPort.c_str());

            if (tmp >= 1 && tmp <= 65536) {
                port = tmp;
            }
        }
 
        if (PR_StringToNetAddr(sIp.c_str(), addr) == PR_FAILURE) {
            char* ip = __Sockets_getHostByName(sIp.c_str());

            if (ip == NULL) {
                return PR_FAILURE;
            }

            sIp = ip; delete [] ip;
 
            if (PR_StringToNetAddr(sIp.c_str(), addr) == PR_FAILURE) {
                return PR_FAILURE;
            }
        }
    }
 
    if (port != -1) {
        PR_InitializeNetAddr(PR_IpAddrNull, port, addr);
    }
 
    return PR_SUCCESS;
}

#endif
