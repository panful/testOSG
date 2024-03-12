/**
 * 1. 绘制一个简单的三角形
 * 2. 简单光照
 * 3. 叶节点
 * 4. 枝节点
 * 5. 相机节点
 * 6. 一个窗口多个相机（从相机），同一个SceneData
 * 7. 多个相机节点使用不同的SceneData
 * 8. CompositeViewer
 * 9. 给可绘制节点添加一个相机节点
 * 10. 给指定的 Drawable 设置属性
 * 11. 绘制指定 Drawable 时清除指定缓冲区
 * 12. 对11添加事件回调，打印相机参数
 */

#define TEST3

#ifdef TEST1

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osgViewer/Viewer>

// Shader.cpp       [664]   glShaderSource
// Shader.cpp       [760]   glCompileShader
// Program.cpp      [1097]  glUseProgram
// BufferObject.cpp [234]   glBufferData
// Uniform.cpp      [2757]  glUniformXXX
// PrimitiveSet.cpp []      glDrawArrays glDrawElements

/**
 * ------------------------------------------------------------------
 * 静态全局变量 static osg::WindowingSystemInterfaceProxy<Win32WindowingSystem> s_proxy_Win32WindowingSystem("Win32");
 * 创建一个当前平台的 WSI(Window system interface)
 *
 * ------------------------------------------------------------------
 * 事件回调
 * LRESULT GraphicsWindowWin32::handleNativeWindowingEvent
 *
 * ------------------------------------------------------------------
 * void Viewer::setSceneData
 *  void View::setSceneData
 *   void View::assignSceneDataToCameras 会将场景图赋给主摄像机 Viewer::_camera 和从相机（由addSlave添加），设置漫游器对应的场景根节点
 * void View::setUpViewInWindow
 *  void SingleWindow::configure 此处需要使用静态全局变量创建的 WSI
 * int Viewer::run
 *  void Viewer::realize
 *   void ViewerBase::setUpThreading
 *    void ViewerBase::startThreading
 *     void GraphicsContext::createGraphicsThread 子线程需要使用上下文绘制
 *  while() 循环
 *   void ViewerBase::frame
 *    void View::init
 *    void Viewer::advance  记录场景的帧数，帧速率信息
 *    void Viewer::eventTraversal()  处理场景的交互事件及其回调
 *    void Viewer::updateTraversal() 处理场景的更新回调，以及分页数据的更新
 *     void Scene::updateSceneGraph(osg::NodeVisitor& updateVisitor) 更新分页数据库的内容
 *      void DatabasePager::updateSceneGraph(const osg::FrameStamp& frameStamp) 去除已经过期的场景子树，向场景图添加新载入的数据
 *     void View::updateSlaves() 更新摄像机组中的所有摄像机投影矩阵
 *    void ViewerBase::renderingTraversals() 场景的渲染遍历工作
 *     signalBeginFrame
 *     void Renderer::cull() 在主线程中向 成员变量 _drawQueue 添加一个元素
 *      void SceneView::cull()
 *     void GraphicsContext::runOperations() 绘制场景
 *     void GraphicsContext::swapBuffers() 双缓冲交换
 *     signalEndFrame
 *
 * ------------------------------------------------------------------
 * void GraphicsThread::run() 其他线程
 *
 * ------------------------------------------------------------------
 * void GraphicsContext::runOperations() 其他线程执行
 *  void Renderer::draw()  有一个 ThreadSafeQueue 类型的成员变量 _drawQueue ，每次从里边拿一个渲染
 *   void SceneView::draw()
 *    void RenderStage::draw
 *     void RenderStage::drawInner
 *      void RenderBin::draw
 *       void RenderStage::drawImplementation
 *        inline bool applyAttribute 设置视口 glViewport
 *        void RenderBin::drawImplementation [487] 遍历所有叶节点并渲染
 *         void RenderLeaf::render
 *          void State::apply
 *           void Shader::PerContextShader::compileShader  编译Shader glCompileShader
 *           void Uniform::apply 设置Uniform
 *          inline void Drawable::draw
 *           void Geometry::drawImplementation
 *            void GLBufferObject::compileBuffer 提交数据 glBufferData
 *            void DrawArrays::draw 绘制 glDrawArrays 文件：PrimitiveSet
 */

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
#include <osg/Light>       // 继承自osg::StateAttribute，保存灯光的模式与属性参数信息
#include <osg/LightSource> // 继承自osg::Group，灯光管理类，继承了osg::Group类的管理节点的接口，将灯光作为一个节点加入到场景图中进行渲染
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

#ifdef TEST3

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osgViewer/Viewer>
#include <vector>

std::vector<osg::ref_ptr<osg::Geometry>> CreateGeometry()
{
    // 创建顶点数组
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-1.f, -1.f, 0.f));
    vertices->push_back(osg::Vec3(1.f, -1.f, 0.f));
    vertices->push_back(osg::Vec3(-1.f, 1.f, 0.f));

    // 创建颜色数组
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
    colors->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
    colors->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));

    // 创建几何体
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geometry->setVertexArray(vertices.get());
    geometry->setColorArray(colors.get());
    geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 3)); // 设置绘制模式为绘制三角形

    // 创建顶点数组
    osg::ref_ptr<osg::Vec3Array> vertices2 = new osg::Vec3Array;
    vertices2->push_back(osg::Vec3(1.f, -1.f, 0.f));
    vertices2->push_back(osg::Vec3(1.f, 1.f, 0.f));
    vertices2->push_back(osg::Vec3(-1.f, 1.f, 0.f));

    // 创建颜色数组
    osg::ref_ptr<osg::Vec4Array> colors2 = new osg::Vec4Array;
    colors2->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
    colors2->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
    colors2->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));

    // 创建几何体
    osg::ref_ptr<osg::Geometry> geometry2 = new osg::Geometry();
    geometry2->setVertexArray(vertices2.get());
    geometry2->setColorArray(colors2.get());
    geometry2->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geometry2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 3)); // 设置绘制模式为绘制三角形

    // 创建顶点数组
    osg::ref_ptr<osg::Vec3Array> vertices3 = new osg::Vec3Array;
    vertices3->push_back(osg::Vec3(-1.f, 1.f, 0.f));
    vertices3->push_back(osg::Vec3(1.f, 1.f, 0.f));
    vertices3->push_back(osg::Vec3(0.f, 2.f, 0.f));

    // 创建颜色数组
    osg::ref_ptr<osg::Vec4Array> colors3 = new osg::Vec4Array;
    colors3->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
    colors3->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
    colors3->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));

    // 创建几何体
    osg::ref_ptr<osg::Geometry> geometry3 = new osg::Geometry();
    geometry3->setVertexArray(vertices3.get());
    geometry3->setColorArray(colors3.get());
    geometry3->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geometry3->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 3)); // 设置绘制模式为绘制三角形

    return { geometry, geometry2, geometry3 };
}

int main()
{
    // 创建一个查看器
    osgViewer::Viewer viewer;

    auto vecGeometries = CreateGeometry();

    // osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    // geode->addDrawable(vecGeometries[0]);
    // geode->addDrawable(vecGeometries[1]);
    // geode->addDrawable(vecGeometries[2]);

    // osg::ref_ptr<osg::Geode> geode2 = new osg::Geode();
    // geode2->addDrawable(vecGeometries[1]);

    // osg::ref_ptr<osg::Geode> geode3 = new osg::Geode();
    // geode3->addDrawable(vecGeometries[2]);

    // 将几何节点添加到场景图
    osg::ref_ptr<osg::Group> root = new osg::Group;
    // root->addChild(geode.get());
    // root->addChild(geode2.get());
    // root->addChild(geode3.get());
    root->addChild(vecGeometries[0]);
    root->addChild(vecGeometries[1]);
    root->addChild(vecGeometries[2]);

    // 设置场景图并运行查看器
    viewer.setSceneData(root.get());

    // 设置窗口大小
    int width  = 800;
    int height = 600;
    viewer.setUpViewInWindow(100, 100, width, height);

    return viewer.run();
}

#endif // TEST3

#ifdef TEST4

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osgViewer/Viewer>
#include <vector>

std::vector<osg::ref_ptr<osg::Geometry>> CreateGeometry()
{
    // 创建顶点数组
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-1.f, -1.f, 0.f));
    vertices->push_back(osg::Vec3(1.f, -1.f, 0.f));
    vertices->push_back(osg::Vec3(-1.f, 1.f, 0.f));

    // 创建颜色数组
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
    colors->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
    colors->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));

    // 创建几何体
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geometry->setVertexArray(vertices.get());
    geometry->setColorArray(colors.get());
    geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 3)); // 设置绘制模式为绘制三角形

    // 创建顶点数组
    osg::ref_ptr<osg::Vec3Array> vertices2 = new osg::Vec3Array;
    vertices2->push_back(osg::Vec3(1.f, -1.f, 0.f));
    vertices2->push_back(osg::Vec3(1.f, 1.f, 0.f));
    vertices2->push_back(osg::Vec3(-1.f, 1.f, 0.f));

    // 创建颜色数组
    osg::ref_ptr<osg::Vec4Array> colors2 = new osg::Vec4Array;
    colors2->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
    colors2->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
    colors2->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));

    // 创建几何体
    osg::ref_ptr<osg::Geometry> geometry2 = new osg::Geometry();
    geometry2->setVertexArray(vertices2.get());
    geometry2->setColorArray(colors2.get());
    geometry2->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geometry2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 3)); // 设置绘制模式为绘制三角形

    // 创建顶点数组
    osg::ref_ptr<osg::Vec3Array> vertices3 = new osg::Vec3Array;
    vertices3->push_back(osg::Vec3(-1.f, 1.f, 0.f));
    vertices3->push_back(osg::Vec3(1.f, 1.f, 0.f));
    vertices3->push_back(osg::Vec3(0.f, 2.f, 0.f));

    // 创建颜色数组
    osg::ref_ptr<osg::Vec4Array> colors3 = new osg::Vec4Array;
    colors3->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
    colors3->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
    colors3->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));

    // 创建几何体
    osg::ref_ptr<osg::Geometry> geometry3 = new osg::Geometry();
    geometry3->setVertexArray(vertices3.get());
    geometry3->setColorArray(colors3.get());
    geometry3->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geometry3->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 3)); // 设置绘制模式为绘制三角形

    return { geometry, geometry2, geometry3 };
}

int main()
{
    // 创建一个查看器
    osgViewer::Viewer viewer;

    // 几何体
    auto vecGeometries = CreateGeometry();

    // 枝节点
    osg::ref_ptr<osg::Group> branch = new osg::Group;
    branch->addChild(vecGeometries[0]);

    // 几何节点
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(vecGeometries[1]);

    // 根节点
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(geode.get());
    root->addChild(branch);
    root->addChild(vecGeometries[2]);

    // 设置场景图并运行查看器
    viewer.setSceneData(root.get());

    // 设置窗口大小
    int width  = 800;
    int height = 600;
    viewer.setUpViewInWindow(100, 100, width, height);

    return viewer.run();
}

#endif // TEST4

#ifdef TEST5

#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

int main()
{
    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Cone> cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);

    osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(cone);
    shapeDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, 1.f));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(shapeDrawable);

    // 相机
    osg::ref_ptr<osg::Camera> camera = new osg::Camera();
    camera->setClearColor(osg::Vec4(.1f, .2f, .3f, 1.f)); // 背景色
    camera->addChild(geode);
    // camera->setViewport(0, 0, 800, 600); // 视口，默认占满整个窗口

    viewer.setSceneData(camera);
    viewer.setUpViewInWindow(100, 100, 800, 600);

    return viewer.run();
}

#endif // TEST5

#ifdef TEST6

#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

#include <iostream>

int main()
{
    osgViewer::Viewer viewer;

    osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    if (!wsi)
    {
        std::cout << "Create window error\n";
        return -1;
    }

    int width { 800 }, height { 600 };

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
    traits->x                                         = 100; // 窗口位置
    traits->y                                         = 100;
    traits->width                                     = width; // 窗口大小
    traits->height                                    = height;
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

    osg::ref_ptr<osg::Camera> camera = new osg::Camera();
    camera->setGraphicsContext(gc.get());
    camera->setViewport(0, 0, 800, 600);
    camera->setDrawBuffer(traits->doubleBuffer ? GL_BACK : GL_FRONT);
    camera->setReadBuffer(traits->doubleBuffer ? GL_BACK : GL_FRONT);
    camera->setClearColor(osg::Vec4(.3f, .2f, .1f, .5f)); // 背景色

    osg::ref_ptr<osg::Camera> camera2 = new osg::Camera();
    camera2->setGraphicsContext(gc.get());
    camera2->setViewport(0, 0, 800, 600);
    camera2->setDrawBuffer(traits->doubleBuffer ? GL_BACK : GL_FRONT);
    camera2->setReadBuffer(traits->doubleBuffer ? GL_BACK : GL_FRONT);
    camera2->setClearColor(osg::Vec4(.1f, .2f, .3f, .5f));

    osg::ref_ptr<osg::Sphere> cone                 = new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(cone);
    shapeDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, 1.f));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(shapeDrawable);

    // 注意设置缩放比例，否则球体可能是扁的
    viewer.addSlave(camera.get(), osg::Matrix::scale(2., 1.0, 1.0), osg::Matrix()); // 添加从相机
    viewer.addSlave(camera2.get(), osg::Matrix::scale(2., 1.0, 1.0), osg::Matrix());
    viewer.setSceneData(geode);

    return viewer.run();
}

#endif // TEST6

#ifdef TEST7

#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

int main()
{
    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Sphere> sphere                = new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(sphere);
    sphereDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, 1.f));

    osg::ref_ptr<osg::Cone> cone                  = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> coneDrawable = new osg::ShapeDrawable(cone);
    coneDrawable->setColor(osg::Vec4(0.f, 1.f, 0.f, 1.f));

    osg::ref_ptr<osg::Camera> camera = new osg::Camera();
    camera->setClearColor(osg::Vec4(.1f, .2f, .3f, 1.f));
    camera->addChild(sphereDrawable);
    camera->setViewport(0, 0, 400, 600);

    osg::ref_ptr<osg::Camera> camera2 = new osg::Camera();
    camera2->setClearColor(osg::Vec4(.3f, .2f, .1f, 1.f));
    camera2->addChild(coneDrawable);
    camera2->setViewport(400, 0, 400, 600);

    // osgViewer::Viewer有一个成员变量 _camera，会将场景图通过 addChild 赋给 _camera
    osg::ref_ptr<osg::Group> root = new osg::Group();
    root->addChild(camera.get());
    root->addChild(camera2.get());

    // 球体宽高比错误，两个视口的交互不一样
    viewer.setSceneData(root);
    viewer.setUpViewInWindow(100, 100, 800, 600);

    return viewer.run();
}

#endif // TEST7

#ifdef TEST8

#include <osg/ShapeDrawable>
#include <osgViewer/CompositeViewer>

#include <iostream>

int main()
{
    osgViewer::CompositeViewer viewer;

    osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    if (!wsi)
    {
        std::cout << "Create window error\n";
        return -1;
    }

    int width { 800 }, height { 600 };

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
    traits->x                                         = 100; // 窗口位置
    traits->y                                         = 100;
    traits->width                                     = width; // 窗口大小
    traits->height                                    = height;
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

    osg::ref_ptr<osg::Sphere> sphere                = new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(sphere);
    sphereDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, .5f));

    osg::ref_ptr<osg::Cone> cone                  = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> coneDrawable = new osg::ShapeDrawable(cone);
    coneDrawable->setColor(osg::Vec4(0.f, 1.f, 0.f, .5f));

    osgViewer::View* view = new osgViewer::View();
    view->getCamera()->setViewport(0, 0, 600, 600);
    view->getCamera()->setClearColor(osg::Vec4(.1, .2, .3, 1.));
    view->getCamera()->setGraphicsContext(gc);
    view->setSceneData(sphereDrawable);

    osgViewer::View* view2 = new osgViewer::View();
    view2->getCamera()->setViewport(200, 0, 600, 600);
    view2->getCamera()->setClearColor(osg::Vec4(0., 0., 0., 0.));
    view2->getCamera()->setGraphicsContext(gc);
    view2->getCamera()->setColorMask(true, true, true, true);
    auto x = view2->getCamera()->getColorMask();

    // osg::ref_ptr<osg::StateSet> stateset = view2->getCamera()->getOrCreateStateSet();
    // stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
    // stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    // stateset->setRenderBinDetails(1, "RenderBin");

    // view2->getCamera()->setFinalDrawCallback(new osg::Camera::DrawCallback());

    view2->setSceneData(coneDrawable);

    viewer.addView(view);
    viewer.addView(view2);

    return viewer.run();
}

#endif // TEST8

#ifdef TEST9

#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

int main()
{
    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Sphere> sphere                = new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(sphere);
    sphereDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, 1.f));

    osg::ref_ptr<osg::Cone> cone                  = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> coneDrawable = new osg::ShapeDrawable(cone);
    coneDrawable->setColor(osg::Vec4(0.f, 1.f, 0.f, 1.f));

    osg::ref_ptr<osg::Camera> camera = new osg::Camera();
    camera->setClearColor(osg::Vec4(.1f, .2f, .3f, 1.f));
    camera->setViewport(400, 0, 400, 600);
    camera->addChild(sphereDrawable);

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(coneDrawable);
    geode->addChild(camera);

    viewer.setSceneData(geode);
    viewer.setUpViewInWindow(100, 100, 800, 600);
    viewer.getCamera()->setViewport(0, 0, 400, 600);

    return viewer.run();
}

#endif // TEST9

#ifdef TEST10

#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

int main()
{
    osg::ref_ptr<osg::Cone> cone                  = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> coneDrawable = new osg::ShapeDrawable(cone);
    coneDrawable->setColor(osg::Vec4(0.f, 1.f, 0.f, .5f));

    osg::ref_ptr<osg::Geode> geode1 = new osg::Geode;
    geode1->addDrawable(coneDrawable);

    osg::ref_ptr<osg::Box> box                   = new osg::Box(osg::Vec3(0.f, 0.f, 0.f), 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);
    boxDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, .5f));

    osg::ref_ptr<osg::Geode> geode2 = new osg::Geode;
    geode2->addDrawable(boxDrawable);

    osg::ref_ptr<osg::Group> root = new osg::Group();
    root->addChild(geode2);
    root->addChild(geode1);

    // 设置圆锥的属性
    // 深度测试、混合、面剔除、线框模式、材质、光照、纹理...
    osg::ref_ptr<osg::StateSet> stateSet = geode1->getOrCreateStateSet();
    stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON); // 开启深度测试，默认开启
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);      // 开启混合，默认关闭

    // 创建查看器并设置场景数据
    osgViewer::Viewer viewer;
    viewer.setSceneData(root);
    viewer.setUpViewInWindow(100, 100, 800, 600);

    // 运行查看器
    return viewer.run();
}

#endif // TEST10

#ifdef TEST11

#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

int main()
{
    osg::ref_ptr<osg::Cone> cone                  = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> coneDrawable = new osg::ShapeDrawable(cone);
    coneDrawable->setColor(osg::Vec4(0.f, 1.f, 0.f, .5f));

    osg::ref_ptr<osg::Geode> geode1 = new osg::Geode;
    geode1->addDrawable(coneDrawable);

    osg::ref_ptr<osg::Box> box                   = new osg::Box(osg::Vec3(0.f, 0.f, 0.f), 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);
    boxDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, .5f));

    osg::ref_ptr<osg::Geode> geode2 = new osg::Geode;
    geode2->addDrawable(boxDrawable);

    osg::ref_ptr<osg::Camera> camera1 = new osg::Camera();
    camera1->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    camera1->addChild(geode1);

    osg::ref_ptr<osg::Camera> camera2 = new osg::Camera();
    camera2->setClearMask(0); // 绘制geode2之前，不清除任何缓冲区
    camera2->addChild(geode2);

    // 默认按照添加的顺序绘制，先添加的先绘制
    // 绘制每一个相机节点时，默认都会清除深度缓冲和颜色缓冲，所以之前的相机渲染结果就会被清除
    osg::ref_ptr<osg::Group> root = new osg::Group();
    root->addChild(camera1);
    root->addChild(camera2); // camera2有一个MVP矩阵，所以和直接添加geode到root会有区别
    // root->addChild(geode1);
    // root->addChild(geode2);

    // 创建查看器并设置场景数据
    osgViewer::Viewer viewer;
    viewer.setSceneData(root);
    viewer.setUpViewInWindow(100, 100, 800, 600);

    // 运行查看器
    return viewer.run();
}

#endif // TEST11

#ifdef TEST12

#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

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
            std::cout << data[0] << '\t' << data[1] << '\t' << data[2] << '\t' << data[3] << '\n'
                      << data[4] << '\t' << data[5] << '\t' << data[6] << '\t' << data[7] << '\n'
                      << data[8] << '\t' << data[9] << '\t' << data[10] << '\t' << data[11] << '\n'
                      << data[12] << '\t' << data[13] << '\t' << data[14] << '\t' << data[16] << '\n';
        };

        switch (ea.getEventType())
        {
            case (osgGA::GUIEventAdapter::PUSH):
            {
                std::cout << "------------------------------------\n";
                auto camView = m_viewer.getCamera()->getViewMatrix();
                auto camProj = m_viewer.getCamera()->getProjectionMatrix();

                std::cout << "Viewer:\nview:\n";
                PrintMat(camView);
                std::cout << "proj:\n";
                PrintMat(camProj);

                // auto view1 = m_camera1->getViewMatrix();
                // auto proj1 = m_camera1->getProjectionMatrix();

                // std::cout << "camera 1:\nview\n";
                // PrintMat(view1);
                // std::cout << "proj\n";
                // PrintMat(proj1);

                // auto view2 = m_camera2->getViewMatrix();
                // auto proj2 = m_camera2->getProjectionMatrix();

                // std::cout << "camera 2:\nview:\n";
                // PrintMat(view2);
                // std::cout << "proj\n";
                // PrintMat(proj2);

                break;
            }
        }

        return false;
    }

    void SetCamera(const osg::ref_ptr<osg::Camera>& c1, const osg::ref_ptr<osg::Camera>& c2)
    {
        m_camera1 = c1;
        m_camera2 = c2;
    }

private:
    osgViewer::Viewer& m_viewer;
    osg::ref_ptr<osg::Camera> m_camera1 { nullptr };
    osg::ref_ptr<osg::Camera> m_camera2 { nullptr };
};
} // namespace

int main()
{
    osg::ref_ptr<osg::Cone> cone                  = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> coneDrawable = new osg::ShapeDrawable(cone);
    coneDrawable->setColor(osg::Vec4(0.f, 1.f, 0.f, .5f));

    osg::ref_ptr<osg::Geode> geode1 = new osg::Geode;
    geode1->addDrawable(coneDrawable);

    osg::ref_ptr<osg::Box> box                   = new osg::Box(osg::Vec3(0.f, 0.f, 0.f), 1.0f);
    osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);
    boxDrawable->setColor(osg::Vec4(1.f, 0.f, 0.f, .5f));

    osg::ref_ptr<osg::Geode> geode2 = new osg::Geode;
    geode2->addDrawable(boxDrawable);

    osg::ref_ptr<osg::Camera> camera1 = new osg::Camera();
    camera1->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    camera1->addChild(geode1);

    osg::ref_ptr<osg::Camera> camera2 = new osg::Camera();
    camera2->setClearMask(0); // 绘制geode2之前，不清除任何缓冲区
    camera2->addChild(geode2);

    // 默认按照添加的顺序绘制，先添加的先绘制
    // 绘制每一个相机节点时，默认都会清除深度缓冲和颜色缓冲，所以之前的相机渲染结果就会被清除
    osg::ref_ptr<osg::Group> root = new osg::Group();
    root->addChild(camera1);
    root->addChild(camera2); // camera2有一个MVP矩阵，所以和直接添加geode到root会有区别
    // root->addChild(geode1);
    // root->addChild(geode2);

    osgViewer::Viewer viewer;
    osg::ref_ptr<MyHandler> eventHandler = new MyHandler(viewer);
    eventHandler->SetCamera(camera1, camera2);

    viewer.setSceneData(root);
    viewer.addEventHandler(eventHandler);
    viewer.setUpViewInWindow(100, 100, 800, 600);

    // 运行查看器
    return viewer.run();
}

#endif // TEST12
