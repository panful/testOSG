
/**
 * 1. 渲染一个OSG自带的几何图形
 * 2. 设置事件处理回调
 * 3. 鼠标左键按下时打印相机参数
 * 4. 使用 createGraphicsContext 创建窗口
 */

#define TEST1

#ifdef TEST1

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

int main()
{
    // 创建一个立方体
    osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0.f, 0.f, 0.f), 1.0f);
    // 圆锥
    osg::ref_ptr<osg::Cone> cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
    // 球体
    osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3(3.0f, 0.0f, 0.0f), 1.0f);

    // 创建一个 ShapeDrawable 以便渲染立方体
    osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(sphere);
    shapeDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, 1.f)); // 设置图形的颜色

    // 将 ShapeDrawable 放入 Geode 中
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(shapeDrawable);

    // 创建 Viewer
    osgViewer::Viewer viewer;
    viewer.setSceneData(geode);                   // 设置 Geode 为场景根节点
    viewer.setUpViewInWindow(100, 100, 800, 600); // 设置窗口大小

    // 开始渲染循环
    return viewer.run();
}

#endif // TEST1

#ifdef TEST2

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <iostream>

namespace {
/// @brief 事件回调
class MyHandler : public osgGA::GUIEventHandler
{
public:
    MyHandler()           = default;
    ~MyHandler() override = default;

    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override
    {
        switch (ea.getEventType())
        {
            case (osgGA::GUIEventAdapter::PUSH):
            {
                std::cout << ea.getButton() << '\n';
                break;
            }
            case (osgGA::GUIEventAdapter::KEYDOWN):
            {
                std::cout << ea.getKey() << '\n';
                break;
            }
        }

        // 返回false表示交由后继的GUIEventHandler处理该事件
        // 返回true表示该事件已经处理完毕，其他事件处理器不会再处理这个事件
        return false;
    }
};
} // namespace

int main()
{
    // 圆锥
    osg::ref_ptr<osg::Cone> cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);

    // 创建一个 ShapeDrawable 以便渲染立方体
    osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(cone);
    shapeDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, 1.f)); // 设置图形的颜色

    // 将 ShapeDrawable 放入 Geode 中
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(shapeDrawable);

    osg::ref_ptr<MyHandler> handler = new MyHandler();

    // 创建 Viewer
    osgViewer::Viewer viewer;
    viewer.setSceneData(geode);                   // 设置 Geode 为场景根节点
    viewer.setUpViewInWindow(100, 100, 800, 600); // 设置窗口大小
    viewer.addEventHandler(handler);              // 对该Viewer添加一个事件处理

    // 开始渲染循环
    return viewer.run();
}

#endif // TEST2

#ifdef TEST3

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <iostream>

namespace {

class MyHandler : public osgGA::GUIEventHandler
{
public:
    MyHandler()           = default;
    ~MyHandler() override = default;

    MyHandler(osgViewer::Viewer& v) : m_viewer(v)
    {
    }

    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override
    {
        static auto PrintMat = [](osg::Matrixd mat)
        {
            auto data = mat.ptr();
            std::cout << "----------------------------------------------------\n";
            std::cout << data[0] << '\t' << data[1] << '\t' << data[2] << '\t' << data[3] << '\n'
                      << data[4] << '\t' << data[5] << '\t' << data[6] << '\t' << data[7] << '\n'
                      << data[8] << '\t' << data[9] << '\t' << data[10] << '\t' << data[11] << '\n'
                      << data[12] << '\t' << data[13] << '\t' << data[14] << '\t' << data[16] << '\n';
        };

        switch (ea.getEventType())
        {
            case (osgGA::GUIEventAdapter::PUSH):
            {
                auto camera = m_viewer.getCamera();
                auto view   = camera->getViewMatrix();
                auto proj   = camera->getProjectionMatrix();

                PrintMat(view);
                PrintMat(proj);

                break;
            }
        }

        return false;
    }

private:
    osgViewer::Viewer& m_viewer;
};
} // namespace

int main()
{
    // 创建 Viewer
    osgViewer::Viewer viewer;

    // 圆锥
    osg::ref_ptr<osg::Cone> cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);

    // 创建一个 ShapeDrawable 以便渲染立方体
    osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(cone);
    shapeDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, 1.f)); // 设置图形的颜色

    // 将 ShapeDrawable 放入 Geode 中
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(shapeDrawable);

    osg::ref_ptr<MyHandler> handler = new MyHandler(viewer);

    viewer.setSceneData(geode);                   // 设置 Geode 为场景根节点
    viewer.setUpViewInWindow(100, 100, 800, 600); // 设置窗口大小
    viewer.addEventHandler(handler);              // 对该Viewer添加一个事件处理

    // 开始渲染循环
    return viewer.run();
}

#endif // TEST3

#ifdef TEST4

#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

#include <iostream>

int main()
{
    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
    traits->x                                         = 100; // 窗口位置
    traits->y                                         = 100;
    traits->width                                     = 800; // 窗口大小
    traits->height                                    = 600;
    traits->windowDecoration                          = true; // 窗口标题
    traits->doubleBuffer                              = true;

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

    if (gc.valid())
    {
        gc->setClearColor(osg::Vec4f(1.f, 1.f, 1.f, 1.f)); // 窗口清除色
        gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        std::cout << "GraphicsWindow has not been created successfully\n";
        return -1;
    }

    osg::ref_ptr<osg::Cone> cone                  = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> coneDrawable = new osg::ShapeDrawable(cone);
    coneDrawable->setColor(osg::Vec4(0.f, 1.f, 0.f, .5f));

    viewer.getCamera()->setViewport(0, 0, 800, 600);
    viewer.getCamera()->setGraphicsContext(gc);
    viewer.setSceneData(coneDrawable);

    return viewer.run();
}

#endif // TEST4
