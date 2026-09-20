#include "mainform.h".h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainForm window;
    // StationWidget window;
    // window.setLabel1("转盘1");
    // window.setTopLabels("测试文本","测试文本","测试文本","测试文本");
    // window.setLeftLabels("测试文本","测试文本","测试文本","测试文本");
    // window.setBottomLabels("测试文本","测试文本","测试文本","测试文本");
    // window.setRightLabels("测试文本","测试文本","测试文本","测试文本");
    window.show();
    return app.exec();
}
