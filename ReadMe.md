## 环境变量配置
OpenSceneGraph_DIR = xxx/OpenSceneGraph/cmake
path 添加 xxx/OpenSceneGraph/bin

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
场景树      SceneGraph
场景子树    Subgraph
节点        Node
摄像机      Camera
渲染器      Renderer
窗口        Window
视口        Viewport
场景        Scene
视图        View
单视景器    Viewer
多视景器    CompositeViewer
漫游器      Manipulator
访问器      Visitor
回调        Callback
事件        Event
更新        Update
筛选        Cull
绘制        Draw