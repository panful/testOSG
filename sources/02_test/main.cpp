/**
 * 1. 绘制一个简单的三角形
 * 2. 简单光照
 */

#define TEST2

#ifdef TEST1

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

#endif // TEST1

#ifdef TEST2

// https://dandelioncloud.cn/article/details/1527470425756155905

#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Light>       //继承自osg::StateAttribute，保存灯光的模式与属性参数信息
#include <osg/LightSource> //继承自osg::Group，灯光管理类，继承了osg::Group类的管理节点的接口，将灯光作为一个节点加入到场景图中进行渲染
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/StateSetManipulator> //事件响应类，对渲染状态进行控制，按w在体/线/点模式间切换，按l在照明与非照明模式下切换，按b在是否开启背面剔除模式下切换，按t在是否开启纹理的情况下切换
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers> //事件监听

/**
 *    在场景中添加光源：
 *    在渲染状态中指定场景模型的法线
 *    在渲染状态中开启光照并允许光源
 *    创建osg::Light对象以定义光源参数
 *    创建光源osg::LightSource对象，将光源参数加入其中
 *    将光源节点添加到场景图形中
 *    场景模型只有在设有单位法线时才会正确显示光照
 */

// 向场景中添加光源
osg::ref_ptr<osg::Group> createLight(osg::ref_ptr<osg::Node> node)
{
    osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
    // 当光照计算结果过于明亮或者暗淡时，允许法线的重缩放
    // stateset->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
    // 场景中的缩放变换非均匀时，允许法线归一化，以保证法线为单位长度
    // stateset->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    // 为获得光照效果允许光照，需要允许光照并至少允许一个光源
    // 开启光照 将ON改为OFF则关闭光照
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    // 允许一个光源GL_LIGHT0
    stateset->setMode(GL_LIGHT0, osg::StateAttribute::ON);
    // 计算包围盒
    osg::BoundingSphere bs;
    node->computeBound();
    bs = node->getBound();
    // 创建一个Light对象，定义光源参数
    osg::ref_ptr<osg::Light> light = new osg::Light();
    light->setLightNum(0);
    // 设置方向
    light->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));
    // 设置位置
    light->setPosition(osg::Vec4(bs.center().x(), bs.center().y(), bs.center().z() + bs.radius(), 1.0f));
    // 设置环境光的颜色
    light->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    // 设置散射光的颜色
    light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    // 设置恒衰减指数
    light->setConstantAttenuation(1.0f);
    // 设置线性衰减指数
    light->setLinearAttenuation(0.0f);
    // 设置二次方衰减指数
    light->setQuadraticAttenuation(0.0f);

    // 创建光源
    osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
    lightSource->setLight(light.get());
    osg::ref_ptr<osg::Group> lightRoot = new osg::Group();
    stateset                           = lightRoot->getOrCreateStateSet();

    lightRoot->addChild(node);
    lightRoot->addChild(lightSource.get());
    return lightRoot.get();
}

int main()
{
    osg::ref_ptr<osg::Node> node  = osgDB::readNodeFile("cow.osg");
    osg::ref_ptr<osg::Group> root = new osg::Group();
    root->addChild(createLight(node.get()));

    // 优化场景数据
    osgUtil::Optimizer optimizer;
    optimizer.optimize(root.get());

    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
    viewer->setSceneData(root.get());
    viewer->setUpViewInWindow(100, 100, 800, 600);

    return viewer->run();
}

#endif // TEST2