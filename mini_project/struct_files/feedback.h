#ifndef FEEDBACK_H
#define FEEDBACK_H
struct Feedback {
    int feedback_id;
    int customer_id;
    char feedback_text[1000];
    bool is_reviewed;
    // int manager_id;   // ID of the manager assigned to review this feedback
    // char manager_response[1000];
};
#endif