#include <iostream>
#include <print>
#include <string>
#include <vector>
#include <optional>

#include "circular_queue.hpp"

class Hospital
{
private:
    CircularQueue<int> patients;
    int nextID = 1;

public:
    explicit Hospital(size_t capacity) : patients(capacity) {}

    bool RegisterPatient()
    {
        if (!patients.enqueue(nextID))
        {
            std::println("Cannot Register: Patient {}, Waiting Room Full", nextID);
            return false;
        }

        std::println("Registered: Patient {}", nextID);
        nextID++;
        return true;
    }

    std::optional<int> ServePatient()
    {
        std::optional<int> id = patients.deque();

        if (id.has_value())
            std::println("Serving: Patient {}", id.value());
        else
            std::println("Serving: No Patient");

        return id;
    }

    void CancelAll()
    {
        while (patients.deque().has_value())
        {
        }

        std::println("All appointments cancelled");
    }

    bool CanDoctorGoHome() const
    {
        return patients.size() == 0;
    }

    void ShowAllPatient() const
    {
        std::vector<int> ids;

        patients.forEach([&ids](const int &id) { //
            ids.push_back(id);
        });

        // insertion sort
        for (size_t i = 1; i < ids.size(); i++)
        {
            int key = ids[i];
            size_t j = i;

            while (j > 0 && ids[j - 1] > key)
            {
                ids[j] = ids[j - 1];
                j--;
            }

            ids[j] = key;
        }

        std::print("Patients List: ");

        if (ids.empty())
        {
            std::println("None");
            return;
        }

        for (size_t i = 0; i < ids.size(); i++)
        {
            if (i > 0)
                std::print(" -> ");

            std::print("({})", ids[i]);
        }

        std::println();
    }
};

// MARK: main
int main()
{
    Hospital hospital(5);

    hospital.ShowAllPatient();

    hospital.RegisterPatient();
    hospital.RegisterPatient();
    hospital.RegisterPatient();
    hospital.ShowAllPatient();

    std::println();

    hospital.ServePatient();
    hospital.ShowAllPatient();

    std::println();

    hospital.RegisterPatient();
    hospital.RegisterPatient();
    hospital.RegisterPatient();
    hospital.RegisterPatient(); // overflow
    hospital.ShowAllPatient();

    std::println();

    hospital.ServePatient();
    hospital.ServePatient();
    hospital.ShowAllPatient();

    std::println("Doctor can go home: {}", hospital.CanDoctorGoHome());

    std::println();

    hospital.CancelAll();
    hospital.ShowAllPatient();

    std::println("Doctor can go home: {}", hospital.CanDoctorGoHome());

    hospital.ServePatient(); // underflow

    std::println();

    hospital.RegisterPatient();
    hospital.ShowAllPatient();

    return 0;
}
