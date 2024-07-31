#include "app/ViewerWindow.h"
#include "core/ArcBall.h"
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <assert.h>
#include <sys/timeb.h>
#include "meshConverter/Mesh3D.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <thread>

ViewerWindow::ViewerWindow()
{
    mesh = new Mesh3D();
}

ViewerWindow::~ViewerWindow(void) {}

void ViewerWindow::renderGeometry() {
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    mesh->draw();
}
// <Florian>
void ViewerWindow::renderGui()
{
    if (isLoading) {
        // Ladefenster anzeigen
        ImGui::Begin("Loading", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Loading, please wait ... ");
        ImGui::End();
    }
    else {
        // Haupt-Benutzeroberfläche
        ImGui::Begin("Options");

        if (ImGui::CollapsingHeader("Control", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Press 1 to display one strip less");
            ImGui::Text("Press 2 to display one more strip");
            ImGui::Text("Press 3 to display all or no stripes");
        }

        // Abstand
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        // Auswahl des Meshes
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginCombo("##combo", items[selectedIndex])) {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                    bool is_selected = (selectedIndex == n);
                    if (ImGui::Selectable(items[n], is_selected)) {
                        selectedIndex = n;
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

        // Abstand
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        // Informationsausgabe
        if (ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Created %d unique vertices", mesh->getVertices().size());
            ImGui::Text("Created %d triangular faces", mesh->getFaces().size());
            ImGui::Text("Created %d stripes", mesh->getStrips().size());

            int total_count_strip_vertices = 0;
            for (const auto& strip : mesh->getStrips()) {
                total_count_strip_vertices += strip.size();
            }

            ImGui::Text("Send %d vertices to GPU instead of %d", total_count_strip_vertices, mesh->getFaces().size() * 3);
            ImGui::Text("Saved %d vertices (%.2f %%)", (mesh->getFaces().size() * 3) - total_count_strip_vertices, (1.0F - (static_cast<float>(total_count_strip_vertices) / (mesh->getFaces().size() * 3))) * 100);
        }

        // Abstand
        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        if (ImGui::Button("Process")) {
            isLoading = true;

            // Starte einen neuen Thread für das Laden des Meshes
            std::thread objThread([this]() {
                std::string filename = std::string("input\\") + items[selectedIndex] + ".obj";

                // Erzeuge ein neues Mesh-Objekt und lese die .obj-Datei
                Mesh3D* newMesh = new Mesh3D();
                newMesh->readOBJ(filename.c_str());

                // Ersetze das alte Mesh durch das neue
                delete mesh;
                mesh = newMesh;

                isLoading = false; // Ladezustand zurücksetzen
                });

            objThread.detach(); // Thread vom Hauptthread trennen
        }

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ViewerWindow::keyEvent(unsigned char key, int x, int y) {
    switch (key) {
    case '1':    // Ein Strip weniger anzeigen
        if (mesh->strip_amount_limit != 0) {
            mesh->strip_amount_limit--;
        }
        break;
    case '2':    // Ein Strip mehr anzeigen
        if (mesh->strip_amount_limit != mesh->getStripsCount()) {
            mesh->strip_amount_limit++;
        }
        break;
    case '3':    // Alle oder keine Stripes anzeigen
        mesh->strip_amount_limit = (mesh->strip_amount_limit > 0) ? 0 : mesh->getStripsCount();
        break;
    }
}
// </Florian>
