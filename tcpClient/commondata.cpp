#include "commondata.h"
#include <QTcpSocket>
#include "ResponseHandler/responseHandler.h"
#include "clientSocketThread.h"

CommonData::CommonData()
{}

long long CommonData::QueryOrSetData(int code,int mode,void* data)
{
    switch (code) {
    case LOCALLANGUAGE:
    {
        if(mode)
        {
            this->strLocalLanguage = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&this->strLocalLanguage);

        }
        break;
    }
    case SCREENRECT:
    {
        if(mode)
        {
            this->objScreenRect = *reinterpret_cast<QRect*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&(this->objScreenRect));

        }
        break;
    }
    case SERVERIP:
    {
        if(mode)
        {
            this->strServerIP = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&(this->strServerIP));

        }
        break;
    }
    case SERVERPORT:
    {
        if(mode)
        {
            this->strServerPort = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&(this->strServerPort));

        }
        break;
    }
    case USERNAME:
    {
        if(mode)
        {
            this->strUserName = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&(this->strUserName));

        }
        break;
    }
    case SERVERPATH:
    {
        if(mode)
        {
            this->strServerPath = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&(this->strServerPath));

        }
        break;
    }
    case STRCURPATH:
    {
        if(mode)
        {
            this->strCurPath = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&(this->strCurPath));

        }
        break;
    }
    case STRROOTPATH:
    {
        if(mode)
        {
            this->strRootPath = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&(this->strRootPath));

        }
        break;
    }
    case PCLIENTSOCKET:
    {
        if(mode)
        {
            this->pClientSocket = reinterpret_cast<QTcpSocket*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(this->pClientSocket);

        }
        break;
    }
    case RESPONSEHANDLE:
    {
        if(mode)
        {
            this->pResponseHandler = reinterpret_cast<responseHandler*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(this->pResponseHandler);

        }
        break;
    }
    case CLIENTSOCKETTHREAD:
    {
        if(mode)
        {
            this->pClientSocketThread = reinterpret_cast<clientSocketThread*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(this->pClientSocketThread);

        }
        break;
    }
    case UPLOADFILEABSOLUTEDNAME:
    {
        if(mode)
        {
            this->strUploadFileAbsolutedName = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&this->strUploadFileAbsolutedName);

        }
        break;
    }
    case PUBLICFILEPATH:
    {
        if(mode)
        {
            this->strPublicFIlePath = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&this->strPublicFIlePath);

        }
        break;
    }
    case WEBADDRESS:
    {
        if(mode)
        {
            this->strWebAddress = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&this->strWebAddress);

        }
        break;
    }
    case QUITMODE:
    {
        if(mode)
        {
            this->strQuitMode = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&this->strQuitMode);

        }
        break;
    }
    case DEFAULTSTOREADDR:
    {
        if(mode)
        {
            this->strDefaultStoreAddr = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&this->strDefaultStoreAddr);

        }
        break;
    }
    case VERSION:
    {
        if(mode)
        {
            this->strVersion = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&this->strVersion);

        }
        break;
    }
    case STYLEMODE:
    {
        if(mode)
        {
            this->strStyleMode = *reinterpret_cast<QString*>(data);
            return true;
        }
        else
        {
            return reinterpret_cast<long long>(&this->strStyleMode);

        }
        break;
    }
    }
    return false;
}

CommonData& CommonData::getInstance()
{
    static CommonData objCommonData; //局部static变量，如果全局区有一个，不会重新创建。
    return objCommonData;
}

