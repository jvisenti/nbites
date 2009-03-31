/**
 * MCL.h
 *
 * @author Tucker Hermans
 * @version %I%
 */

#ifndef MCL_h_DEFINED
#define MCL_h_DEFINED

// Includes
// STL
#include <vector>
#include <math.h>
#include <time.h> // for srand(time(NULL))
#include <cstdlib> // for MAX_RAND
// Local
#include "Observation.h"
#include "FieldConstants.h"
#include "EKFStructs.h"
#include "NBMath.h"
#include "NogginStructs.h"
#include "LocSystem.h"

// Particle
class Particle
{
public:
    Particle(PoseEst _pose, float _weight);
    Particle(const Particle& other);
    Particle();
    PoseEst pose;
    float weight;

    friend std::ostream& operator<< (std::ostream &o, const Particle &c) {
        return o << c.pose.x << " " << c.pose.y << " " << c.pose.h << " "
                 << c.weight;
    }

};

// Constants
static const int M = 100; // Number of particles
static const float MIN_SIMILARITY = 1.0e-15; // Minimum possible similarity

// The Monte Carlo Localization class
class MCL : public LocSystem
{
public:
    // Constructors & Destructors
    MCL();
    virtual ~MCL();

    // Core Functions
    virtual void updateLocalization(MotionModel u_t,
                                    std::vector<Observation> z_t);

    // Getters
    const PoseEst getCurrentEstimate() const { return curEst; }
    const PoseEst getCurrentUncertainty() const { return curUncert; }
    /**
     * @return The current x esitamte of the robot
     */
    const float getXEst() const { return curEst.x;}

    /**
     * @return The current y esitamte of the robot
     */
    const float getYEst() const { return curEst.y;}

    /**
     * @return The current heading esitamte of the robot in radians
     */
    const float getHEst() const { return curEst.h;}

    /**
     * @return The current heading esitamte of the robot in degrees
     */
    const float getHEstDeg() const { return curEst.h * DEG_OVER_RAD;}

    /**
     * @return The uncertainty associated with the x estimate of the robot.
     */
    const float getXUncert() const { return curUncert.x * 2;}

    /**
     * @return The uncertainty associated with the y estimate of the robot.
     */
    const float getYUncert() const { return curUncert.y * 2;}

    /**
     * @return The uncertainty associated with the robot's heading estimate.
     */
    const float getHUncert() const { return curUncert.h * 2;}

    /**
     * @return The uncertainty associated with the robot's heading estimate.
     */
    const float getHUncertDeg() const { return curUncert.h * 2 * DEG_OVER_RAD;}

    /**
     * @return The current set of particles in the filter
     */
    const std::vector<Particle> getParticles() const { return X_t; }

    // Setters
    /**
     * @param xEst The current x esitamte of the robot
     */
    void setXEst(float xEst) { curEst.x = xEst;}

    /**
     * @param yEst The current y esitamte of the robot
     */
    void setYEst(float yEst) { curEst.y = yEst;}

    /**
     * @param hEst The current heading esitamte of the robot
     */
    void setHEst(float hEst) { curEst.h = hEst;}

    /**
     * @param uncertX The uncertainty of the x estimate of the robot.
     */
    void setXUncert(float uncertX) { curUncert.x = uncertX;}

    /**
     * @return uncertY The uncertainty of the y estimate of the robot.
     */
    void setYUncert(float uncertY) { curUncert.y = uncertY;}

    /**
     * @param uncertH The uncertainty of the robot's heading estimate.
     */
    void setHUncert(float uncertH) { curUncert.h = uncertH;}

private:
    // Class variables
    PoseEst curEst; // Current {x,y,h} esitamates
    PoseEst curUncert; // Associated {x,y,h} uncertainties (standard deviations)
    std::vector<Particle> X_t; // Current set of particles

    // Core Functions
    PoseEst updateMotionModel(PoseEst x_t, MotionModel u_t);
    float updateMeasurementModel(std::vector<Observation> z_t, PoseEst x_t);
    void updateEstimates();

    // Helpers
    float determinePointWeight(Observation z, PoseEst x_t,
                               PointLandmark landmark);
    float determineLineWeight(Observation z, PoseEst x_t, LineLandmark _line);
    float getSimilarity(float r_d, float r_a, Observation &z);
    Particle randomWalkParticle(Particle p);
    float sampleNormalDistribution(float sd);
    float sampleTriangularDistribution(float sd);

public:
    friend std::ostream& operator<< (std::ostream &o, const MCL &c) {
        return o << "Est: " << c.curEst << "\nUnct: " << c.curUncert;
    }

};

#endif // _MCL_H_DEFINED
