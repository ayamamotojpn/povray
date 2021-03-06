//******************************************************************************
///
/// @file backend/scene/objects.h
///
/// This module contains all defines, typedefs, and prototypes for
/// `objects.cpp`.
///
/// @note   `frame.h` contains other object stuff.
///
/// @copyright
/// @parblock
///
/// Persistence of Vision Ray Tracer ('POV-Ray') version 3.7.
/// Copyright 1991-2015 Persistence of Vision Raytracer Pty. Ltd.
///
/// POV-Ray is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Affero General Public License as
/// published by the Free Software Foundation, either version 3 of the
/// License, or (at your option) any later version.
///
/// POV-Ray is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Affero General Public License for more details.
///
/// You should have received a copy of the GNU Affero General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// ----------------------------------------------------------------------------
///
/// POV-Ray is based on the popular DKB raytracer version 2.12.
/// DKBTrace was originally written by David K. Buck.
/// DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
///
/// @endparblock
///
//******************************************************************************

#ifndef OBJECTS_H
#define OBJECTS_H

#include "core/material/texture.h"

#include "backend/bounding/bbox.h"

namespace pov
{

/*****************************************************************************
* Global preprocessor defines
******************************************************************************/

/*
 * [DB 7/94]
 *
 * The flag field is used to store all possible flags that are
 * used for objects (up to 32).
 *
 * The flages are manipulated using the following macros:
 *
 *   Set_Flag    (Object, Flag) : set    specified Flag in Object
 *   Clear_Flag  (Object, Flag) : clear  specified Flag in Object
 *   Invert_Flag (Object, Flag) : invert specified Flag in Object
 *   Test_Flag   (Object, Flag) : test   specified Flag in Object
 *
 *   Copy_Flag   (Object1, Object2, Flag) : Set the Flag in Object1 to the
 *                                          value of the Flag in Object2.
 *   Bool_Flag   (Object, Flag, Bool)     : if(Bool) Set flag else Clear flag
 *
 * Object is a pointer to the object.
 * Flag is the number of the flag to test.
 *
 */

#define NO_SHADOW_FLAG            0x00000001L /* Object doesn't cast shadows            */
#define CLOSED_FLAG               0x00000002L /* Object is closed                       */
#define INVERTED_FLAG             0x00000004L /* Object is inverted                     */
#define SMOOTHED_FLAG             0x00000008L /* Object is smoothed                     */
#define CYLINDER_FLAG             0x00000010L /* Object is a cylinder                   */
#define DEGENERATE_FLAG           0x00000020L /* Object is degenerate                   */
#define STURM_FLAG                0x00000040L /* Object should use sturmian root solver */
#define OPAQUE_FLAG               0x00000080L /* Object is opaque                       */
#define MULTITEXTURE_FLAG         0x00000100L /* Object is multi-textured primitive     */
#define INFINITE_FLAG             0x00000200L /* Object is infinite                     */
#define HIERARCHY_FLAG            0x00000400L /* Object can have a bounding hierarchy   */
#define HOLLOW_FLAG               0x00000800L /* Object is hollow (atmosphere inside)   */
#define HOLLOW_SET_FLAG           0x00001000L /* Hollow explicitly set in scene file    */
#define UV_FLAG                   0x00002000L /* Object uses UV mapping                 */
#define DOUBLE_ILLUMINATE_FLAG    0x00004000L /* Illuminate both sides of the surface   */
#define NO_IMAGE_FLAG             0x00008000L /* Object doesn't catch camera rays     [ENB 9/97] */
#define NO_REFLECTION_FLAG        0x00010000L /* Object doesn't catch reflection rays [ENB 9/97] */
#define NO_GLOBAL_LIGHTS_FLAG     0x00020000L /* Object doesn't receive light from global lights */
#define NO_GLOBAL_LIGHTS_SET_FLAG 0x00040000L /* Object doesn't receive light from global lights explicitly set in scene file */
/* Photon-related flags */
#define PH_TARGET_FLAG            0x00080000L /* this object is a photons target */
#define PH_PASSTHRU_FLAG          0x00100000L /* this is pass through object (i.e. it may let photons pass on their way to the target) */
#define PH_RFL_ON_FLAG            0x00200000L /* this object explicitly reflects photons */
#define PH_RFL_OFF_FLAG           0x00400000L /* this object explicitly does not reflect photons */
#define PH_RFR_ON_FLAG            0x00800000L /* this object explicitly refracts photons */
#define PH_RFR_OFF_FLAG           0x01000000L /* this object explicitly does not refract photons */
#define PH_IGNORE_PHOTONS_FLAG    0x02000000L /* this object does not collect photons */
#define IGNORE_RADIOSITY_FLAG     0x04000000L /* Object doesn't receive ambient light from radiosity */
#define NO_RADIOSITY_FLAG         0x08000000L /* Object doesn't catch radiosity rays (i.e. is invisible to radiosity) */
#define CUTAWAY_TEXTURES_FLAG     0x10000000L /* Object (or any of its parents) has cutaway_textures set */



#define Set_Flag(Object, Flag)     \
    { (Object)->Flags |=  (Flag); }

#define Clear_Flag(Object, Flag)   \
    { (Object)->Flags &= ~(Flag); }

#define Invert_Flag(Object, Flag)  \
    { (Object)->Flags ^=  (Flag); }

#define Test_Flag(Object, Flag)    \
    ( (Object)->Flags & (Flag))

#define Copy_Flag(Object1, Object2, Flag) \
    { (Object1)->Flags = (((Object1)->Flags) & (~Flag)) | \
                         (((Object2)->Flags) &  (Flag)); }

#define Bool_Flag(Object, Flag, Bool) \
    { if(Bool){ (Object)->Flags |=  (Flag); } else { (Object)->Flags &= ~(Flag); }}



/* Object types. */

#define BASIC_OBJECT                0
#define PATCH_OBJECT                1 /* Has no inside, no inverse */
#define TEXTURED_OBJECT             2 /* Has texture, possibly in children */
#define IS_COMPOUND_OBJECT          4 /* Has children field */
#define STURM_OK_OBJECT             8 /* STURM legal */
//#define WATER_LEVEL_OK_OBJECT      16 /* WATER_LEVEL legal */
#define LIGHT_SOURCE_OBJECT        32 /* link me in frame.light_sources */
#define BOUNDING_OBJECT            64 /* This is a holder for bounded object */
//#define SMOOTH_OK_OBJECT          128 /* SMOOTH legal */
#define IS_CHILD_OBJECT           256 /* Object is inside a COMPOUND */
/* NK 1998 - DOUBLE_ILLUMINATE is not used anymore - use DOUBLE_ILLUMINATE_FLAG */
#define HIERARCHY_OK_OBJECT       512 /* NO_HIERARCHY legal */
#define LT_SRC_UNION_OBJECT      1024 /* Union of light_source objects only */
#define LIGHT_GROUP_OBJECT       2048 /* light_group union object [trf] */
#define LIGHT_GROUP_LIGHT_OBJECT 4096 /* light in light_group object [trf] */
#define CSG_DIFFERENCE_OBJECT    8192 /* csg difference object */
#define IS_CSG_OBJECT           16384 /* object is a csg and not some other compound object */
#define CHILDREN_FLAGS (PATCH_OBJECT+TEXTURED_OBJECT)  /* Reverse inherited flags */


/*****************************************************************************
* Classes
******************************************************************************/

class MessageFactory;

/// Abstract base class for all geometric objects.
class ObjectBase
{
    public:
        int Type; // TODO - make obsolete
        TEXTURE *Texture;
        TEXTURE *Interior_Texture;
        InteriorPtr interior;
        vector<ObjectPtr> Bound;
        vector<ObjectPtr> Clip;
        vector<LightSource *> LLights;  ///< Used for light groups.
        BoundingBox BBox;
        TRANSFORM *Trans;
        TRANSFORM *UV_Trans;
        SNGL Ph_Density;
        FloatSetting RadiosityImportance;
        unsigned int Flags;

#ifdef OBJECT_DEBUG_HELPER
        ObjectDebugHelper Debug;
#endif

        /// Construct object from scratch.
        ObjectBase(int t) :
            Type(t),
            Texture(NULL), Interior_Texture(NULL), interior(), Trans(NULL), UV_Trans(NULL),
            Ph_Density(0), RadiosityImportance(0.0), Flags(0)
        {
            Make_BBox(BBox, -BOUND_HUGE/2.0, -BOUND_HUGE/2.0, -BOUND_HUGE/2.0, BOUND_HUGE, BOUND_HUGE, BOUND_HUGE);
        }

        /// Construct object as copy of existing one.
        ///
        /// @param[in]  t           Object type.
        /// @param[in]  o           Object to copy data from.
        /// @param[in]  transplant  Whether to move data rather than copy it.
        ///
        ObjectBase(int t, ObjectBase& o, bool transplant) :
            Type(t),
            Texture(o.Texture), Interior_Texture(o.Interior_Texture), interior(o.interior), Trans(o.Trans), UV_Trans(o.UV_Trans),
            Ph_Density(o.Ph_Density), RadiosityImportance(o.RadiosityImportance), Flags(o.Flags),
            Bound(o.Bound), Clip(o.Clip), LLights(o.LLights), BBox(o.BBox)
        {
            if (transplant)
            {
                o.Texture = NULL;
                o.Interior_Texture = NULL;
                o.interior.reset();
                o.Trans = NULL;
                o.UV_Trans = NULL;
                o.Bound.clear();
                o.Clip.clear();
                o.LLights.clear();
            }
        }
        virtual ~ObjectBase() { }

        virtual ObjectPtr Copy() = 0;

        virtual bool All_Intersections(const Ray&, IStack&, TraceThreadData *) = 0; // could be "const", if it wasn't for isosurface max_gradient estimation stuff
        virtual bool Inside(const Vector3d&, TraceThreadData *) const = 0;
        virtual void Normal(Vector3d&, Intersection *, TraceThreadData *) const = 0;
        virtual void UVCoord(Vector2d&, const Intersection *, TraceThreadData *) const;
        virtual void Translate(const Vector3d&, const TRANSFORM *) = 0;
        virtual void Rotate(const Vector3d&, const TRANSFORM *) = 0;
        virtual void Scale(const Vector3d&, const TRANSFORM *) = 0;
        virtual void Transform(const TRANSFORM *) = 0;

        /// Invert the object.
        ///
        /// @attention  This method may return a newly constructed object and destroy itself.
        ///
        virtual ObjectPtr Invert();

        virtual void Compute_BBox() = 0;
        virtual void Determine_Textures(Intersection *, bool, WeightedTextureVector&, TraceThreadData *Thread); // could be "(const Intersection*...) const" if it wasn't for blob specials

        /// Checks whether a given ray intersects the object's bounding box.
        /// Primitives with low-cost intersection tests may override this to always return true
        virtual bool Intersect_BBox(BBoxDirection, const BBoxVector3d&, const BBoxVector3d&, BBoxScalar = HUGE_VAL) const;

        /// Optional post-render message dispatcher.
        ///
        /// This method will be called upon completion of rendering a view. This is the appropriate
        /// place to send messages that would traditionally have been sent at the end of a render or
        /// at object destruction - e.g. IsoSurface max_gradient warnings. (object destruction isn't
        /// the place to do that anymore since a scene may persist between views).
        ///
        virtual void DispatchShutdownMessages(MessageFactory& messageFactory) {};

    protected:
        explicit ObjectBase(const ObjectBase&) { }
};

/// Convenience class to derive patch objects from.
class NonsolidObject : public ObjectBase
{
    public:
        NonsolidObject(int t) : ObjectBase(t) {}
        virtual ObjectPtr Invert();
};

/// Abstract base class for compound geometric objects.
///
/// @note   Some special compound objects (such as mesh or blob) do _not_ derive from this class.
///
class CompoundObject : public ObjectBase
{
    public:
        CompoundObject(int t) : ObjectBase(t) {}
        CompoundObject(int t, CompoundObject& o, bool transplant) : ObjectBase(t, o, transplant), children(o.children) { if (transplant) o.children.clear(); }
        vector<ObjectPtr> children;
        virtual ObjectPtr Invert();
};


/// Light source.
class LightSource : public CompoundObject
{
    public:
        size_t index;
        MathColour colour;
        Vector3d Direction, Center, Points_At, Axis1, Axis2;
        DBL Coeff, Radius, Falloff;
        DBL Fade_Distance, Fade_Power;
        int Area_Size1, Area_Size2;
        int Adaptive_Level;
        ObjectPtr Projected_Through_Object;

        unsigned Light_Type : 8;
        bool Area_Light : 1;
        bool Use_Full_Area_Lighting : 1; // JN2007: Full area lighting
        bool Jitter : 1;
        bool Orient : 1;
        bool Circular : 1;
        bool Parallel : 1;
        bool Photon_Area_Light : 1;
        bool Media_Attenuation : 1;
        bool Media_Interaction : 1;
        bool lightGroupLight : 1;

        LightSource();
        virtual ~LightSource();

        virtual ObjectPtr Copy();

        virtual bool All_Intersections(const Ray&, IStack&, TraceThreadData *);
        virtual bool Inside(const Vector3d&, TraceThreadData *) const;
        virtual void Normal(Vector3d&, Intersection *, TraceThreadData *) const;
        virtual void UVCoord(Vector2d&, const Intersection *, TraceThreadData *) const;
        virtual void Translate(const Vector3d&, const TRANSFORM *);
        virtual void Rotate(const Vector3d&, const TRANSFORM *);
        virtual void Scale(const Vector3d&, const TRANSFORM *);
        virtual void Transform(const TRANSFORM *);
        virtual void Compute_BBox() {}
};


/// Generic abstract class used for containing inherently infinite objects (isosurface, parametric).
struct ContainedByShape
{
    virtual ~ContainedByShape() {}

    virtual void ComputeBBox(BoundingBox& rBox) const = 0;
    virtual bool Intersect(const Ray& ray, const TRANSFORM* pTrans, DBL& rDepth1, DBL& rDepth2, int& rSide1, int& sSide2) const = 0;
    virtual bool Inside(const Vector3d& IPoint) const = 0;
    virtual void Normal(const Vector3d& IPoint, const TRANSFORM* pTrans, int side, Vector3d& rNormal) const = 0;
    virtual ContainedByShape* Copy() const = 0;
};

/// Class used for containing inherently infinite objects (isosurface, parametric) in a box.
struct ContainedByBox : public ContainedByShape
{
    Vector3d corner1;
    Vector3d corner2;

    ContainedByBox() : corner1(-1,-1,-1), corner2(1,1,1) {}

    virtual void ComputeBBox(BoundingBox& rBox) const;
    virtual bool Intersect(const Ray& ray, const TRANSFORM* pTrans, DBL& rDepth1, DBL& rDepth2, int& rSide1, int& sSide2) const;
    virtual bool Inside(const Vector3d& IPoint) const;
    virtual void Normal(const Vector3d& IPoint, const TRANSFORM* pTrans, int side, Vector3d& rNormal) const;
    virtual ContainedByShape* Copy() const;
};

/// Class used for containing inherently infinite objects (isosurface, parametric) in a sphere.
struct ContainedBySphere : public ContainedByShape
{
    Vector3d center;
    DBL radius;

    ContainedBySphere() : center(0,0,0), radius(1) {}

    virtual void ComputeBBox(BoundingBox& rBox) const;
    virtual bool Intersect(const Ray& ray, const TRANSFORM* pTrans, DBL& rDepth1, DBL& rDepth2, int& rSide1, int& sSide2) const;
    virtual bool Inside(const Vector3d& IPoint) const;
    virtual void Normal(const Vector3d& IPoint, const TRANSFORM* pTrans, int side, Vector3d& rNormal) const;
    virtual ContainedByShape* Copy() const;
};


/*****************************************************************************
* Global functions
******************************************************************************/

bool Find_Intersection(Intersection *Ray_Intersection, ObjectPtr Object, const Ray& ray, TraceThreadData *Thread);
bool Find_Intersection(Intersection *Ray_Intersection, ObjectPtr Object, const Ray& ray, const RayObjectCondition& postcondition, TraceThreadData *Thread);
bool Find_Intersection(Intersection *isect, ObjectPtr object, const Ray& ray, BBoxDirection variant, const BBoxVector3d& origin, const BBoxVector3d& invdir, TraceThreadData *ThreadData);
bool Find_Intersection(Intersection *isect, ObjectPtr object, const Ray& ray, BBoxDirection variant, const BBoxVector3d& origin, const BBoxVector3d& invdir, const RayObjectCondition& postcondition, TraceThreadData *ThreadData);
bool Ray_In_Bound(const Ray& ray, const vector<ObjectPtr>& Bounding_Object, TraceThreadData *Thread);
bool Point_In_Clip(const Vector3d& IPoint, const vector<ObjectPtr>& Clip, TraceThreadData *Thread);
ObjectPtr Copy_Object(ObjectPtr Old);
vector<ObjectPtr> Copy_Objects(vector<ObjectPtr>& Src);
void Translate_Object(ObjectPtr Object, const Vector3d& Vector, const TRANSFORM *Trans);
void Rotate_Object(ObjectPtr Object, const Vector3d& Vector, const TRANSFORM *Trans);
void Scale_Object(ObjectPtr Object, const Vector3d& Vector, const TRANSFORM *Trans);
void Transform_Object(ObjectPtr Object, const TRANSFORM *Trans);
bool Inside_Object(const Vector3d& IPoint, ObjectPtr Object, TraceThreadData *Thread);
void Destroy_Object(vector<ObjectPtr>& Object);
void Destroy_Object(ObjectPtr Object);
void Destroy_Single_Object(ObjectPtr *ObjectPtr);

}

#endif
