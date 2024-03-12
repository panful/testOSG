## 环境变量配置
OpenSceneGraph_DIR = xxx/OpenSceneGraph/cmake
path 添加 xxx/OpenSceneGraph/bin

## GL3
[osg下GL3环境搭建](https://zhuanlan.zhihu.com/p/539708040)
编译完成后，将下载的GL和KHR文件夹拷贝到xxx/OpenSceneGraph/include，第三方库bin目录下所有文件拷贝到 xxx/OpenSceneGraph/bin

## Osg源码分析
osg::Geode和osg::Geometry区别是：osg::Geode继承自osg::Group，可以包含多个子节点；osg::Geometry和osg::Group都继承自osg::Node，但是osg::Geometry不可以包含子节点，只是一个独立节点。
osg::StateSet：此类封装了OpenGL状态参数，实现对OpenGL参数的集中管理，封装成类，更方便使用。

参数解析器：osg::ArgumentParser arguments(&argc, argv);
模型读写：osgDB::writeNodeFile
创建相机并配置：osg::Camera

osgViewer
它是OSG核心的一部分，位于OSG Core/osgViewer，用来渲染3D场景并处理交互事件。
给视图设置模型：osgViewer::Viewer::setSceneData()，参数为3D场景根节点。

osgViewer::addEventHandler：给视图添加事件处理器。
osgViewer::StatsHandler：统计数据显示类，也是继承自osgGA::GUIEventHandler，事件驱动统计。按下’s’显示OSG实时运行参数的统计信息，就是由这个类实现的。
osgViewer::ViewerBase：这个类集成了整个osg渲染过程，需要特别关注。仔细阅读其代码！

osgGA
这个项目位于OSG Core/osgViewer，是OSG核心的一部分。
它实现了事件通知机制，定义了响应3D场景中各种事件的回调接口。大概有25个类，不多，值得花点时间看看。

osgGA::EventHandler：事件处理基类，里面有一些遍历的代码，不太好懂。理解它唯一的方法是调试。打上断点，对着当前这个例子调试就完事了！
osgGA::GUIEventHandler：是osgGA::EventHandler的子类，通过调试理解。
osgGA::StateSetManipulator：是osgGA::GUIEventHandler的子类，通过调试理解。
osgGA::EventVisitor：继承自osg::NodeVisitor，也还是包含遍历的代码，通过调试理解。
osgGA::StateSetManipulator：此类顾名思义是用于实现用户操作osg::StateSet的，它通过响应用户输入，修改场景或节点的渲染状态，比如按下字母’l’控制场景灯光，按下字母’t’切换纹理，按下字母‘w’切换显示模式。

多线程渲染
osg::Operation
osg::OperationThread
多线程渲染的原理是使用后台线程绘制，然后主线程在帧刷新时同步绘制。
## Osg源码分析 最长的一帧
[最长的一帧](https://www.docin.com/p-173577521.html)
### 场景树      SceneGraph
### 场景子树    Subgraph
### 节点        Node
有一个类型为 osg::StateSet 的成员变量 _stateset
### 摄像机      Camera
osg::Camera 有一个类型为 osg::GraphicsOperation 的成员变量 _renderer
### 渲染器      Renderer
Renderer 继承自 osg::GraphicsOperation
有一个类型为 ThreadSafeQueue 的成员变量 _availableQueue，一个类型为 ThreadSafeQueue 的成员变量 _drawQueue
有一个类型为 osgUtil::SceneView 的成员变量 _sceneView[2]
ThreadSafeQueue 有一个类型为 `std::list<osgUtil::SceneView*>` 的成员变量 _queue
osgUtil::SceneView 注释中表示该类已被弃用（开发人员不再使用该类，内部代码仍然需要调用）
场景的筛选(cull)、绘制(draw)实质上都是由 osgUtil::SceneView 完成
osgUtil::RenderStage 继承自 osgUtil::RenderBin
osgUtil::RenderBin 有一个类型为 `std::vector<RenderLeaf*>` 的成员变量 _renderLeafList，类型为 `std::vector<StateGraph*>` 的成员变量 _stateGraphList
osgUtil::StateGraph 有一个类型为`std::vector<osg::ref_ptr<RenderLeaf>>`的成员变量 _leaves
### 窗口        Window
### 视口        Viewport
### 场景        Scene
osgViewer::Scene 有一个类型为 osg::Node 的成员变量 _sceneData
### 视图        View
osgViewer::View 有一个类型为 osgViewer::Scene 的成员变量 _scene
osgViewer::View 继承自 osg::View 和 osgGA::GUIActionAdapter
osg::View 有一个类型为 osg::Camera 的成员变量 _camera;
### 单视景器    Viewer
osgViewer::Viewer 继承自 ViewerBase 和 osgViewer::View
### 多视景器    CompositeViewer
### 访问器      Visitor
### 回调        Callback
### 事件        Event
osgGA::GUIEventAdapter 表示各种类型的鼠标、键盘、触压笔和窗口事件
继承 osgGA::GUIEventHandler 并重写 handle 函数获取实时的鼠标、键盘输入，可以自定义用户的交互
漫游器 Manipulator 交互模式，控制方式。即输入的键盘、鼠标事件如何控制相机
### 更新        Update
### 筛选        Cull
### 绘制        Draw
