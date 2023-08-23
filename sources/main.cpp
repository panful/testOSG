
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

int main()
{
    // 创建 Viewer
    osgViewer::Viewer viewer;

    // 创建一个立方体
    osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0, 0, 0), 1.0f);

    // 创建一个 ShapeDrawable 以便渲染立方体
    osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(box);

    // 将 ShapeDrawable 放入 Geode 中
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(shapeDrawable);

    // 设置 Geode 为场景根节点
    viewer.setSceneData(geode);

    // 设置窗口大小
    int width  = 800;
    int height = 600;
    viewer.setUpViewInWindow(100, 100, width, height);

    // 开始渲染循环
    return viewer.run();
}