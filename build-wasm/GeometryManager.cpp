#include "GeometryManager.hpp"
#include "Geometry.hpp"
#include "LCRSTree.hpp"
#include "WasmOcctView.hpp"

// OCCT
#include <BinXCAFDrivers.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <Message.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_TreeNode.hxx>
#include <XCAFDoc.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDataStd_Name.hxx>
#include <TopLoc_Location.hxx>
#include <Quantity_Color.hxx>
#include <AIS_ColoredShape.hxx>
#include <Prs3d_LineAspect.hxx>
#include <TDF_ChildIterator.hxx>

// Standard Libraries
#include <iostream>
#include <utility>


GeometryManager::GeometryManager()
{
    m_pGeometryTree = new LCRSTree<Geometry>();

    m_hXCAFApp = XCAFApp_Application::GetApplication();
    m_hStdDoc = nullptr;

    m_hXCAFApp->NewDocument("BinXCAF", m_hStdDoc);
    BinXCAFDrivers::DefineFormat(m_hXCAFApp);
}

GeometryManager::~GeometryManager()
{
    if (m_pGeometryTree == nullptr) return;
    delete m_pGeometryTree;
    m_pGeometryTree = nullptr;
}

bool GeometryManager::ImportStepFile(const char* fileName, std::istream& istream)
{
    if(!GetOCCDocFromStepFile(fileName, istream)) {
        return false;
    }
    
    // Load Geometry From OCC Document
    bool retStatus = LoadGeometryFromOCCDoc();

    return retStatus;
}

bool GeometryManager::GetOCCDocFromStepFile(const char* fileName, std::istream& istream)
{
    Message::DefaultMessenger()->Send(fileName, Message_Warning);
    
    STEPCAFControl_Reader readerCAF;
    STEPControl_Reader& reader = readerCAF.ChangeReader();
    
    if (XCAFDoc_DocumentTool::IsXCAFDocument(m_hStdDoc)) {
        IFSelect_ReturnStatus status = reader.ReadStream(fileName, istream);
        if (status != IFSelect_RetDone) {
            switch (status)
		    {
		    case IFSelect_RetError:
                Message::DefaultMessenger()->Send("Not a valid Step file", Message_Warning);
    			return false;
		    case IFSelect_RetFail:
                Message::DefaultMessenger()->Send("Reading has failed", Message_Warning);
			    return false;
		    case IFSelect_RetVoid:
                Message::DefaultMessenger()->Send("Nothing to transfer", Message_Warning);
		    	return false;
		    default:
			    return false;
                // Qualifies an execution status :
                // RetVoid  : normal execution which created nothing, or no data to process
                // RetDone  : normal execution with a result
                // RetError : error in command or input data, no execution
                // RetFail  : execution was run and has failed
                // RetStop  : indicates end or stop (such as Raise)
		    }
        }
        if (!readerCAF.Transfer(m_hStdDoc)) {  // Transfer reader data to m_hStdDoc
            Message::DefaultMessenger()->Send("Cannot read any relevant data from the STEP file", Message_Warning);
            return false;
        }
        Message::DefaultMessenger()->Send("Read STEP File done!", Message_Warning);
        return true;
    }
    return false;
}

bool GeometryManager::LoadGeometryFromOCCDoc()
{
    TDF_Label mainLabel = m_hStdDoc->Main();
    TDF_Label shapeLabel = mainLabel.FindChild(mainLabel.Tag(), false);
    int shapeTag = shapeLabel.Tag();

    Geometry rootGeometry("Root");
    GEOMETRY_NODE rootNode = m_pGeometryTree->InsertItem(std::move(rootGeometry));
    
    TopLoc_Location location;
    location.Identity();  // Set location to identity matrix

    IterateFather(shapeLabel, rootNode, shapeTag, location);


    //std::string s = std::to_string(mainLabel.Tag());
    Message::DefaultMessenger()->Send(TCollection_AsciiString(mainLabel.Tag()).ToCString(), Message_Warning);
    //TCollection_AsciiString entryStr;
	//TDF_Tool::Entry(mainLabel, entryStr);
    //Message::DefaultMessenger()->Send(entryStr.ToCString(), Message_Warning);
    Message::DefaultMessenger()->Send(GetEntryString(mainLabel).ToCString(), Message_Warning);

    
    Message::DefaultMessenger()->Send(GetEntryString(shapeLabel).ToCString(), Message_Warning);

    return true;
}

void GeometryManager::IterateFather(const TDF_Label& label, GEOMETRY_NODE node, const int tag, TopLoc_Location loc, bool callByTree)
{
    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(label);	
	Handle(TDataStd_TreeNode) tree;

    if (label.FindAttribute(XCAFDoc::ShapeRefGUID(), tree)) { // If this label has TreeNode
		if (tree->HasFirst()) { // This Label is reference label
			if (!callByTree) {
				return; // If reference label found, break this loop. (assume that if the first reference found all assemblies are operated.)
			}
		}
		if (tree->HasFather()) {
			// Calculate Location
			TopLoc_Location thisLocation;
			thisLocation = shapeTool->GetLocation(label);
			thisLocation = loc * thisLocation;
			IterateFather(tree->Father()->Label(), node, tag, thisLocation, true); // call by tree
			return;
		}
	}

    GEOMETRY_NODE currentNode = AddGeometryToTree(label, node, tag, loc);

	if (label.HasChild()) {
		//Node<Geometry>* np = geometryTree->GetThisNode();
		for (TDF_ChildIterator itall(label, Standard_False); itall.More(); itall.Next()) {
			TDF_Label childLabel = itall.Value();
			Standard_Integer childTag = childLabel.Tag();			
			IterateFather(childLabel, currentNode, childTag, loc);
		}
	}
}

GeometryManager::GEOMETRY_NODE GeometryManager::AddGeometryToTree(const TDF_Label& label, GEOMETRY_NODE node, const int tag, TopLoc_Location loc)
{
    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(label);
	Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(label);
	Handle(TDataStd_TreeNode) tree;
	TopoDS_Shape aShape;
	//CString entryStr, nameStr;
	//Standard_Integer aTag = label.Tag();
	bool isSubShape = false;
	
    TCollection_AsciiString entryStr = GetEntryString(label);
    std::string nameStr = GetNameString(label).ToCString();
	//OutputDebugString(_T("Entry: ") + CString(entryStr) + _T(", Name: ") + CString(nameStr) + _T("\n"));

    TCollection_AsciiString entryNameStr = TCollection_AsciiString("Entry: ") + entryStr + TCollection_AsciiString(", Name: ") + TCollection_AsciiString(nameStr.c_str()) + "\n";
    Message::DefaultMessenger()->Send(entryNameStr.ToCString(), Message_Warning);

	if (nameStr != std::string("Shapes") && entryStr != TCollection_AsciiString("0:1:1")) { // If this Label is not 'Shape Label'
		aShape = shapeTool->GetShape(label);
		// If TDataStd_Name is Empty
		if (nameStr.empty()) {
			std::string geomName = node->GetData().GetName(); // Parent Name		
			std::string geomNum = std::to_string(tag);
            // CString geomNum;
			// geomNum.Format(_T("%d"), tag);
			nameStr = geomName + geomNum;
		}		
		// Make this Geometry Structure
		//Geometry geom(tag, nameStr, entryStr);
        Geometry geom(nameStr.c_str());
		//SubGeometry subGeom;
		if (!aShape.IsNull()) {			
			Quantity_Color col(Quantity_NOC_LIGHTGRAY); // Default Color is Light Gray
			//if (aShape.ShapeType() == TopAbs_FACE) {
			//	subGeom.SetShape(aShape);
			//	isSubShape = true;
			//}
			// Color Handling
			if (label.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorSurf), tree)) {
				if (tree->HasFather()) {
					TDF_Label colorLabel = tree->Father()->Label();
					XCAFDoc_ColorType ctype = XCAFDoc_ColorSurf;
					if (colorTool->GetColor(colorLabel, col)) {
						//if(isSubShape) subGeom.SetColor(col);
						//else geom.SetColor(col);
                        geom.SetColor(col);
					}
				}
			}
			if (label.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen), tree)) {
				if (tree->HasFather()) {
					TDF_Label colorLabel = tree->Father()->Label();
					// TODO: SOMETHING NEEDED
				}
			}
			if (label.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorCurv), tree)) {
				if (tree->HasFather()) {
					TDF_Label colorLabel = tree->Father()->Label();
					// TODO: SOMETHING NEEDED
				}
			}
			//if (!isSubShape) {
				Handle(AIS_ColoredShape) shape = new AIS_ColoredShape(aShape);
				// Locate this shape by calculated location data
				shape->SetShape(shape->Shape().Located(loc));
				shape->SetColor(col); // Set Color to AIS_Shape
				// Set Line Color by Shape's Color
				Handle(Prs3d_LineAspect) lineAspect = new Prs3d_LineAspect(col, Aspect_TOL_SOLID, 2.0);
				shape->Attributes()->SetFaceBoundaryAspect(lineAspect);
				shape->Attributes()->SetFaceBoundaryDraw(Standard_True);
                geom.SetColor(col);
				geom.SetShape(shape);
			//}			
		}
		//if (!isSubShape) {
            //lastGeometryIndex++; // Index of The Root Geometry Is 0
            //geom.SetIndex(lastGeometryIndex);
			//geometryTree->AddChild(node, geom);
            //if (geom.GetShape()->Shape().ShapeType() == TopAbs_SOLID) {
                return m_pGeometryTree->InsertItem(geom, node);
            //}
            //else {
            //    return node;
            //}
		//}
		//else {
		//	if (!subGeom.IsEmpty()) {
		//		node->GetData().AddSubGeometry(subGeom); // Add SubGeometry to Parent Node
		//	}
		//}
	}
    return node;
}

TCollection_AsciiString GeometryManager::GetEntryString(const TDF_Label& label)
{
    TCollection_AsciiString entryStr;
    TDF_Tool::Entry(label, entryStr);
    return entryStr;
}

TCollection_AsciiString GeometryManager::GetNameString(const TDF_Label& label)
{
	Handle(TDataStd_Name) name = new TDataStd_Name();
	TCollection_AsciiString nameStr;
	if (label.FindAttribute(TDataStd_Name::GetID(), name))
	{
		nameStr = name->Get();
	}
	return nameStr;    
}

void GeometryManager::PrintIDName(GEOMETRY_NODE node, int depth) {
    TCollection_AsciiString dash;
    for (int i = 0; i < depth; i++) dash += "-";

    TCollection_AsciiString lbr("["), rbr("]");
    TCollection_AsciiString id(node->GetData().GetID());
    TCollection_AsciiString name(node->GetData().GetName().c_str());

    Message::DefaultMessenger()->Send(dash + name + lbr + id + rbr, Message_Info);
}

void GeometryManager::PrintAllGeometryName()
{
    m_pGeometryTree->LoopTree(m_pGeometryTree->GetRoot(), GeometryManager::PrintIDName);
}

void GeometryManager::DisplayGeometry(GEOMETRY_NODE node, int depth)
{
    Handle(AIS_ColoredShape) shape = node->GetData().GetShape();
    WasmOcctView& viewer = WasmOcctView::Instance();

    if (!shape.IsNull()) {
        const TopoDS_Shape aShape = shape->Shape();
        if (aShape.ShapeType() == TopAbs_SOLID) {
            viewer.Context()->Display(shape, false);
        }
    }
}

void GeometryManager::DisplayAllGeometry()
{
    m_pGeometryTree->LoopTree(m_pGeometryTree->GetRoot(), GeometryManager::DisplayGeometry);
}

void GeometryManager::SelectFace(GEOMETRY_NODE node, int depth)
{
    Handle(AIS_ColoredShape) shape = node->GetData().GetShape();
    WasmOcctView& viewer = WasmOcctView::Instance();

    if (!shape.IsNull()) {
        const TopoDS_Shape aShape = shape->Shape();
        if (aShape.ShapeType() == TopAbs_SOLID) {
            viewer.Context()->Deactivate(shape, AIS_Shape::SelectionMode(viewer.GetSelectionMode()));
            viewer.Context()->Activate(shape, AIS_Shape::SelectionMode(TopAbs_FACE));
            viewer.SetSelectionMode(TopAbs_FACE);
        }
    }
}

void GeometryManager::SelectFaceMode()
{
    m_pGeometryTree->LoopTree(m_pGeometryTree->GetRoot(), GeometryManager::SelectFace);

    //WasmOcctView& viewer = WasmOcctView::Instance();
    //viewer.Context()->Deactivate();
    //viewer.Context()->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));

    Message::DefaultMessenger()->Send(TCollection_AsciiString("FaceMode"), Message_Info);
}

void GeometryManager::SelectSolid(GEOMETRY_NODE node, int depth)
{
    Handle(AIS_ColoredShape) shape = node->GetData().GetShape();
    WasmOcctView& viewer = WasmOcctView::Instance();

    if (!shape.IsNull()) {
        const TopoDS_Shape aShape = shape->Shape();
        if (aShape.ShapeType() == TopAbs_SOLID) {
            viewer.Context()->Deactivate(shape, AIS_Shape::SelectionMode(viewer.GetSelectionMode()));
            viewer.Context()->Activate(shape, AIS_Shape::SelectionMode(TopAbs_SOLID));
            viewer.SetSelectionMode(TopAbs_SOLID);
        }
    }
}

void GeometryManager::SelectSolidMode()
{
    m_pGeometryTree->LoopTree(m_pGeometryTree->GetRoot(), GeometryManager::SelectSolid);

    //WasmOcctView& viewer = WasmOcctView::Instance();
    //viewer.Context()->Deactivate();
    //viewer.Context()->Activate(AIS_Shape::SelectionMode(TopAbs_SOLID));

    Message::DefaultMessenger()->Send(TCollection_AsciiString("SolidMode"), Message_Info);
}