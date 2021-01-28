#include "CustomLogo.h"
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <unistd.h>



CustomLogo::CustomLogo(QObject *parent)
	: QObject(parent)
{
	
}


void CustomLogo::setServiceState(QString name, int port, bool state) {
	mServices[name] = QPair<int, int>(port, state);
}


QString CustomLogo::getLogo() {
	struct winsize size;
	QStringList infos;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	int cols = size.ws_col;



	infos << "";	
	//infos << servicesInfo();
	infos << "";	
	infos << devicesInfo();
	
	return toLogo(infos);
}
	

QStringList CustomLogo::servicesInfo() {
	QStringList infos;

	infos << putAtmiddle(10, "SERVICE")+putAtmiddle(10, "PORT")+putAtmiddle(10, "STATUS");
	for (QMap<QString, QPair<int, bool>>::iterator it=mServices.begin(); it != mServices.end(); it++)
		infos << putAtmiddle(10, it.key())+putAtmiddle(10, QString::number(it.value().first))+putAtmiddle(10, it.value().second ? "online" : "offline");

	return infos;
}




QStringList CustomLogo::devicesInfo() {
	QPair<qint64, qint64> mem = getMemoryPercent();
	QPair<qint64, qint64> swap = getSwapPercent();
	int cpu = getCpuPercent();
	QStringList infos;

	infos << putAtmiddle(10, "DEVICE")+putAtmiddle(10, "TOTAL")+putAtmiddle(10, "USAGE");

	//MEMORY INFOMATIONS
	
	infos << putAtmiddle(10, "mem")+putAtmiddle(10, QString::number(mem.first)+" M")+
		putAtmiddle(10, QString::number(mem.second)+"%");

	//SWAP INFORMATIONS

	infos << putAtmiddle(10, "swap")+putAtmiddle(10, QString::number(swap.first)+" M")+
		putAtmiddle(10, QString::number(swap.second)+"%");

	//CPU INFOMATIONS

	infos << putAtmiddle(10, "cpu")+putAtmiddle(10, QString::number(get_nprocs())+" CPU")+
		putAtmiddle(10, QString::number(cpu)+"%");

	return infos;
}







QString CustomLogo::repeatCh(int size, char ch) {
	QString ret;
	ret.resize(size);
	ret.fill(ch);
	return ret;
}


QString CustomLogo::putAtmiddle(int size, QString s) {
	QString ret;
	int p = (size/2)-(s.length()/2);
	for (int x = 0; x <= size; x++) {
		if (x == p) {
			x += s.length();
			ret += s;
		} else
			ret += ' ';
	}
	return ret;
}



QString CustomLogo::toLogo(QStringList list) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	int cols = size.ws_col;
	QString logo;

	//logo = putAtmiddle(cols, repeatCh(cols/2-2, '-'))+"\n";
	//logo += putAtmiddle(cols, repeatCh(cols/2, ' '))+"\n";

	for (QString text : list)
		logo += putAtmiddle(cols, putAtmiddle(cols/2, text))+"\n";


	//logo += putAtmiddle(cols, "|"+repeatCh(cols/2-2, ' ')+"|")+"\n";
	//logo += putAtmiddle(cols, repeatCh(cols/2-2, '-'))+"\n";

	return logo;
	
}



QPair<qint64, qint64> CustomLogo::getMemoryPercent() {
	struct sysinfo mem;
	sysinfo(&mem);
	qint64 total = mem.totalram/1024000;
	qint64 free = total-(mem.freeram/1024000);
	return QPair<qint64, qint64>(total, 100*free/total);
}



QPair<qint64, qint64> CustomLogo::getSwapPercent() {
	struct sysinfo mem;
	sysinfo(&mem);
	if (mem.totalswap == 0)
		return QPair<qint64, qint64>(0,0);

	qint64 total = mem.totalswap/1024000;
	qint64 free = total-(mem.freeswap/1024000);
	return QPair<qint64, qint64>(total, 100*free/total);
}


int CustomLogo::getCpuPercent() {
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;
    unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
        &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
            (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;

}


