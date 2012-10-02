#ifndef VTKTRACERINTERACTORSTYLE_H
#define VTKTRACERINTERACTORSTYLE_H

#include <vtkImageTracerWidget.h>
#include <vtkInteractorStyleImage.h>
#include <vtkSmartPointer.h>
#include <vtkImageActor.h>

class vtkTracerInteractorStyle : public vtkInteractorStyleImage
{

public:

    static vtkTracerInteractorStyle* New();
    vtkTypeMacro(vtkTracerInteractorStyle, vtkInteractorStyleImage);
    vtkTracerInteractorStyle();

    void initTracer(vtkSmartPointer<vtkImageActor> imageActor);

private:

    void clearTracer();
    void catchEvent(vtkObject* caller, long unsigned int eventId, void* callData);

    vtkSmartPointer<vtkImageTracerWidget> tracer;

};


#endif // VTKTRACERINTERACTORSTYLE_H