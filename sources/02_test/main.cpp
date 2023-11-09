#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osgViewer/Viewer>

int main()
{
    // 创建一个查看器
    osgViewer::Viewer viewer;

    // 创建一个几何节点
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();

    // 创建顶点数组
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-1.0f, 0.0f, 0.0f));
    vertices->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
    vertices->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));

    // 创建颜色数组
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); // 红色
    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); // 绿色
    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); // 蓝色

    // 创建几何体
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geometry->setVertexArray(vertices.get());
    geometry->setColorArray(colors.get());
    geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    // 设置绘制模式为绘制三角形
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 3));

    // 将几何体添加到几何节点
    geode->addDrawable(geometry.get());

    // 将几何节点添加到场景图
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(geode.get());

    // 设置场景图并运行查看器
    viewer.setSceneData(root.get());

    // 设置窗口大小
    int width  = 800;
    int height = 600;
    viewer.setUpViewInWindow(100, 100, width, height);

    return viewer.run();
}
